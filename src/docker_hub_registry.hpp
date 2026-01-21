#pragma once

#include "abstract_registry.hpp"

#include <string>
#include <string_view>
#include <unordered_set>

class DockerHubRegistry : public AbstractRegistry {
public:
    /**
     * Explicit constructor
     */
    explicit DockerHubRegistry()
        : AbstractRegistry{ "https://hub.docker.com" }
    {}

    /**
     * Whether this registry matches the container image registry
     */
    virtual bool matchContainerImage(const ContainerImage &image) const override;

    /**
     * Fetch tags for container image from registry
     */
    virtual std::unordered_set<std::string> fetchTags(const ContainerImage &image) override;
};
