#pragma once

#include <httplib.h>
#include <nlohmann/json.hpp>

#include <string>
#include <string_view>
#include <unordered_set>
#include <memory>
#include <expected>

class ContainerImage;

class AbstractRegistry {
protected:
    std::unique_ptr<httplib::Client> m_client{};

    /**
     * Root location where API can be found
     */
    std::string m_api_location{};

    /**
     * Initialize common things between all registries
     * @param api_host      scheme://host[:port] of the registry API e.g. https://hub.docker.com
     * @param api_location  Potential location of the API endpoint (defaults to root "/")
     */
    explicit AbstractRegistry(std::string_view api_host, std::string_view api_location = "/");

    /**
     * Perform a GET request, expecting a JSON response
     * Or a string describing the error on error
     * TODO: handle rate limits
     */
    std::expected<nlohmann::json, std::string> getJSON(std::string_view endpoint);

public:
    /**
     * virtual destructor
     */
    virtual ~AbstractRegistry() = default;

    /**
     * Whether this registry matches the container image registry
     */
    virtual bool matchContainerImage(const ContainerImage &image) const = 0;

    /**
     * Fetch tags for container image from registry
     */
    virtual std::unordered_set<std::string> fetchTags(const ContainerImage &image) = 0;
};
