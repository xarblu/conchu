#include "docker_hub_registry.hpp"

#include <string>
#include <string_view>
#include <unordered_set>


bool DockerHubRegistry::matchSource(std::string_view source) const {
    return source.starts_with("docker.io/");
}

std::unordered_set<std::string> DockerHubRegistry::fetchTags(std::string_view source) {
    std::unordered_set<std::string> tags{};

    return tags;
}
