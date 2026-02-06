#pragma once

#include <memory>
#include <stop_token>
#include <mutex>
#include <condition_variable>
#include <thread>

class Config;
class ContainerEngine;
class DockerHubRegistry;

int daemon_init(std::shared_ptr<Config> config);

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

    /**
     * Vars for watcherThread
     */
    std::condition_variable m_watchCondition{};
    std::mutex m_watchMutex{};

public:
    explicit Daemon(std::shared_ptr<Config> config)
        : m_config{config}
    {}

    /**
     * Start the main daemon
     */
    void start();

private:
    /**
     * The container watcher thread entry point
     */
    void watcherThread(std::stop_token stoken);
};
