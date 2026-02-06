#include "mode_daemon.hpp"

#include "abstract_registry.hpp"
#include "config.hpp"
#include "container_engine.hpp"
#include "docker_hub_registry.hpp"
#include "tag_filter.hpp"

#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <functional>
#include <stop_token>
#include <format>
#include <condition_variable>

int daemon_init(std::shared_ptr<Config> config) {
    Daemon daemon{config};
    daemon.start();

    return 0;
}

void Daemon::start() {
    m_watcherThread = std::jthread{std::bind(&Daemon::watcherThread, this, std::placeholders::_1)};

    m_watcherThread.join();
}

void Daemon::watcherThread(std::stop_token stoken) {
    std::cout << std::format("Container watcher thread started (Interval: {})\n", m_config->checkInterval());

    std::mutex mtx{};
    std::condition_variable cond{};
    bool trigger{false};

    std::jthread triggerer{[this, &mtx, &cond, &trigger] (std::stop_token stoken) {
        while (!stoken.stop_requested()) {
            std::unique_lock lock{mtx};
            cond.wait(lock, [&trigger] { return trigger; });
            trigger = false;
            triggerWatchEvent();
            mtx.unlock();
        }
    }};

    while (!stoken.stop_requested()) {
        {
            std::lock_guard lock{mtx};
            trigger = true;
        }
        cond.notify_one();
        std::this_thread::sleep_for(m_config->checkInterval());
    }
}

void Daemon::triggerWatchEvent() {
    std::cout << "Checking containers...\n";

    ContainerEngine engine{m_config->containerEngineHost()};
    DockerHubRegistry docker_hub{};

    auto containers = engine.getContainers();
    if (containers.empty()) {
        std::cout << "No containers found\n";
        return;
    }

    for (const auto &container : containers) {
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
        } else {
            std::cout << "Could not find latest tag - No tags?\n";
        }
    }
}
