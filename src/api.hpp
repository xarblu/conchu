#pragma once

#include <memory>

namespace httplib {
    class Request;
    class Response;
    class Server;
}

class Config;

class Api {
private:
    std::shared_ptr<Config> m_config{nullptr};
    std::unique_ptr<httplib::Server> m_server{nullptr};

public:
    /**
     * Construct Api handler
     */
    explicit Api(const std::shared_ptr<Config> config);

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
