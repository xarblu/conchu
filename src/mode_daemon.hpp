#pragma once

#include "container.hpp"
#include "tag.hpp"

#include <memory>
#include <stop_token>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <utility>
#include <mutex>

class Config;
class ContainerEngine;
class DockerHubRegistry;

int daemon_init(const std::shared_ptr<Config> config);

class Daemon {
private:
    /**
     * Shared vars
     */
    std::shared_ptr<Config> m_config{nullptr};

    /**
     * Thread trackers
     */
    std::jthread m_watcherThread;
    std::jthread m_apiThread;

    /**
     * Vars for watcherThread
     */
    std::condition_variable m_watchCondition{};
    std::mutex m_watchMutex{};

    /**
     * Collected containers
     */
    std::vector<std::pair<Container, Tag>> m_containers{};
    std::mutex m_containers_mtx{};

public:
    explicit Daemon(std::shared_ptr<Config> config)
        : m_config{config}
    {}

    /**
     * Start the main daemon
     */
    void start();

    /**
     * Shutdown daemon
     */
    void shutdown();

    /**
     * Get a copy of the current Container->Tag mappings
     */
    std::vector<std::pair<Container, Tag>> getContainers();

private:
    /**
     * The container watcher thread entry point
     */
    void watcherThread(std::stop_token stoken);

    /**
     * The HTTP API thread entry point
     */
    void apiThread(std::stop_token stoken);
};
