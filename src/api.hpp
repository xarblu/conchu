#pragma once

#include <memory>

namespace httplib {
    class Request;
    class Response;
    class Server;
}

class Config;
class Daemon;

class Api {
private:
    /**
     * Pointer to the owning Daemon
     */
    Daemon* m_daemon{nullptr};

    /**
     * Shared pointer to the config
     */
    std::shared_ptr<Config> m_config{nullptr};

    /**
     * HTTP Server
     */
    std::unique_ptr<httplib::Server> m_server{nullptr};

public:
    /**
     * Construct Api handler
     */
    explicit Api(Daemon* daemon, const std::shared_ptr<Config> config);

    /**
     * Start listening for requests
     */
    void start();

    /**
     * Stop the server
     */
    void stop();

    /**
     * /api/containers
     */
    void handleContainersGET(const httplib::Request &, httplib::Response &res);
};
