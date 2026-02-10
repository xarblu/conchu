#include "api.hpp"

#include "config.hpp"
#include "mode_daemon.hpp"

#include <httplib.h>
#include <nlohmann/json.hpp>

#include <memory>
#include <functional>
#include <iostream>

Api::Api(Daemon* daemon, const std::shared_ptr<Config> config) {
    m_daemon = daemon,
    m_config = config;

    m_server = std::make_unique<httplib::Server>();
    m_server->Get("/api/containers", std::bind(&Api::handleContainersGET, this, std::placeholders::_1, std::placeholders::_2));
}

void Api::start() {
    if (!m_server->is_valid()) {
        std::cout << "Server invalid\n";
        return;
    }

    m_server->listen(m_config->listenAddr(), m_config->listenPort());
}

void Api::stop() {
    m_server->stop();
}

void Api::handleContainersGET(const httplib::Request &, httplib::Response &res) {
    nlohmann::json json = nlohmann::json::array();
    int idx{0};

    for (const auto&[container, tag] : m_daemon->getContainers()) {
        auto container_json = container.toJSON();

        // TODO: Inject into Container in Daemon?
        //       That way container.toJSON() can handle it
        container_json["result"]["tag"] = tag.raw();

        json[idx++] = container_json;
    }

    res.set_content(json.dump(), "application/json");
}
