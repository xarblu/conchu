#include "mode_daemon.hpp"

#include "abstract_registry.hpp"
#include "api.hpp"
#include "config.hpp"
#include "container_engine.hpp"
#include "docker_hub_registry.hpp"
#include "tag_filter.hpp"

#include <chrono>
#include <condition_variable>
#include <csignal>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <stop_token>
#include <thread>
#include <utility>
#include <vector>

volatile static std::sig_atomic_t gSignalStatus;

int daemon_init(const std::shared_ptr<Config> config) {
    // signals triggering shutdown
    std::signal(SIGINT, [](int signal) {gSignalStatus = signal;});
    std::signal(SIGTERM, [](int signal) {gSignalStatus = signal;});

    Daemon daemon{config};
    daemon.start();

    return 0;
}

void Daemon::start() {
    m_watcherThread = std::jthread{std::bind(&Daemon::watcherThread, this, std::placeholders::_1)};
    m_apiThread = std::jthread{std::bind(&Daemon::apiThread, this, std::placeholders::_1)};

    // TODO: proper thread pool with parallel cancellation
    while (gSignalStatus != SIGINT && gSignalStatus != SIGTERM) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    shutdown();
}

void Daemon::shutdown() {
    std::cout << std::format("Shutdown requested\n");

    m_watcherThread.request_stop();
    m_apiThread.request_stop();

    m_watcherThread.join();
    m_apiThread.join();
}

void Daemon::watcherThread(std::stop_token stoken) {
    std::cout << std::format("Container watcher thread started (Interval: {})\n", m_config->checkInterval());

    std::mutex mtx{};
    std::condition_variable cond{};

    std::stop_callback cb{stoken, [&cond] {
        cond.notify_one();
    }};

    ContainerEngine engine{m_config->containerEngineHost()};
    DockerHubRegistry docker_hub{};

    while (true) {
        std::unique_lock lock{mtx};
        cond.wait_for(lock, m_config->checkInterval(), [&stoken]{return stoken.stop_requested();});
        if (stoken.stop_requested()) {
            break;
        }

        std::cout << "Checking containers...\n";

        auto containers = engine.getContainers();
        if (containers.empty()) {
            std::cout << "No containers found\n";
        }

        for (const auto &container : containers) {
            if (stoken.stop_requested()) {
                break;
            }

            std::cout << "Found Container: " << container.toString() << "\n";

            if (!container.image().has_value()) {
                std::cout << "Skipping due to empty image attribute\n";
                continue;
            }

            ContainerImage image{container.image().value()};
            AbstractRegistry* registry{nullptr};

            if (docker_hub.matchContainerImage(image)) {
                std::cout << "Matched registry Docker Hub\n";
                registry = &docker_hub;
            }

            if (!registry){
                std::cout << "No registry matched\n";
                continue;
            }

            auto tags = registry->fetchTags(image);

            TagFilter tag_filter{tags, std::regex{"\\d+\\.\\d+\\.\\d+"}};
            auto latest_tag = tag_filter.latest();
            if (latest_tag.has_value()) {
                std::cout << "Latest tag: " << latest_tag.value().raw() << "\n";
                {
                    std::lock_guard lock{m_containers_mtx};
                    std::pair<Container, Tag> entry{Container{container}, Tag{latest_tag.value()}};

                    // erase existing container
                    // ensures we only ever have the latest one
                    bool present{false};
                    for (int idx{0}; idx < m_containers.size(); idx++) {
                        if (m_containers[idx].first == entry.first) {
                            present = true;
                            m_containers.erase(m_containers.begin() + idx--);
                            break;
                        }
                    }

                    if (present) {
                        std::cout << std::format("Updating container: {}\n", entry.first.toString());
                    } else {
                        std::cout << std::format("Adding container: {}\n", entry.first.toString());
                    }

                    m_containers.push_back(std::move(entry));
                }
            } else {
                std::cout << "Could not find latest tag - No tags?\n";
            }
        }

        // cleanup non-present containers
        {
            std::lock_guard lock{m_containers_mtx};
            for (int idx{0}; idx < m_containers.size(); idx++) {
                bool present{false};
                for (auto& container : containers) {
                    if (m_containers[idx].first == container) {
                        present = true;
                        break;
                    }
                }
                if (!present) {
                    std::cout << std::format("Removing container: {}\n", m_containers[idx].first.toString());
                    m_containers.erase(m_containers.begin() + idx--);
                }
            }
        }
    }

    std::cout << "Container watcher thread stopped\n";
}

void Daemon::apiThread(std::stop_token stoken) {
    std::cout << std::format("HTTP API thread started (Serving from: http://{}:{})\n", m_config->listenAddr(), m_config->listenPort());

    Api api{this, m_config};
    std::stop_callback cb(stoken, [&api]{api.stop();});
    api.start();

    std::cout << "HTTP API thread stopped\n";
}

std::vector<std::pair<Container, Tag>> Daemon::getContainers() {
    std::lock_guard lock{m_containers_mtx};
    return m_containers;
}
