#include "abstract_registry.hpp"

#include <httplib.h>
#include <nlohmann/json.hpp>

#include <memory>
#include <string>
#include <string_view>
#include <expected>
#include <format>

AbstractRegistry::AbstractRegistry(std::string_view api_host, std::string_view api_location) {
    m_client = std::make_unique<httplib::Client>(std::string(api_host));
    m_api_location = api_location;
}

std::expected<nlohmann::json, std::string> AbstractRegistry::getJSON(std::string_view endpoint) {
    if (!m_client) {
        return std::unexpected{"m_client is nullptr"};
    }

    std::string location = m_api_location;
    if (!location.ends_with("/") && !endpoint.starts_with("/")) {
        location += "/";
    }
    location += endpoint;

    auto res = m_client->Get(location);
    if (!res) {
        return std::unexpected{std::format("Error calling GET {}: {}", location, httplib::to_string(res.error()))};
    }

    if (res->status < 200 || res->status >299) {
        return std::unexpected{std::format("Error calling GET {}: Status {}: {}", location, res->status, res->body)};
    }

    if (!nlohmann::json::accept(res->body)) {
        return std::unexpected{std::format("Invalid JSON from GET {}: {}", location, res->body)};
    }

    return nlohmann::json::parse(res->body);
}
