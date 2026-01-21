#include "container_engine.hpp"

#include <httplib.h>
#include <nlohmann/json.hpp>

#include <ostream>
#include <string>
#include <memory>
#include <iostream>
#include <format>
#include <iomanip>
#include <vector>

#include <sys/socket.h>

ContainerEngine::ContainerEngine(std::string host) {
    if (std::string_view scheme{"unix://"}; host.starts_with(scheme)) {
        auto socket = host.substr(scheme.size());

        std::cerr << std::format("Connecting to {} via UNIX socket", socket) << std::endl;
        m_client = std::make_unique<httplib::Client>(socket);
        m_client->set_address_family(AF_UNIX);

    } else if (host.starts_with("http://") || host.starts_with("https://")) {
        std::cerr << std::format("Connecting to {} via TCP/IP socket", host) << std::endl;
        m_client = std::make_unique<httplib::Client>(host);

    } else {
        std::cerr << "Unsupported scheme for host: " << host << std::endl;
    }
}

bool ContainerEngine::isValid() const {
    if (!m_client) {
        return false;
    }

    auto ping = m_client->Get("/_ping");
    if (!ping) {
        std::cerr << "Error calling " << std::quoted("/_ping")
                  << ": " << ping.error() << std::endl;
        return false;
    }

    if (ping->status == httplib::StatusCode::OK_200) {
        return true;
    }

    return false;
}

std::vector<Container> ContainerEngine::getContainers() const {
    std::vector<Container> containers{};

    if (!isValid()) {
        return containers;
    }
    
    auto res = m_client->Get("/containers/json");
    if (!res) {
        std::cerr << "Error calling " << std::quoted("/containers/json")
                  << ": " << res.error() << std::endl;
        return containers;
    }

    if (res->status != httplib::StatusCode::OK_200) {
        std::cerr << "Error calling " << std::quoted("/containers/json")
                  << ": Code " << res->status  << std::endl;
        return containers;
    }

    auto json = nlohmann::json::parse(res->body);

    if (!json.is_array()) {
        std::cerr << "Received invalid JSON from container engine (expected array):\n"
                  << json.dump(2) << std::endl;
        return containers;
    }

    for (auto &container : json) {
        containers.push_back(Container{container});
    }

    return containers;
}
