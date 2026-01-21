#include "docker_hub_registry.hpp"
#include "abstract_registry.hpp"
#include "container_image.hpp"

#include <nlohmann/json.hpp>

#include <format>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_set>


bool DockerHubRegistry::matchContainerImage(const ContainerImage &image) const {
    return image.registry()
                .transform([](std::string registry) {return registry == "docker.io";})
                .value_or(false);
}

std::unordered_set<std::string> DockerHubRegistry::fetchTags(const ContainerImage &image) {
    std::unordered_set<std::string> tags{};

    if (!image.cnamespace().has_value()) {
        std::cerr << "Missing attribute 'namespace' in ContainerImage: " << image.toString() << std::endl;
        return tags;
    }
    if (!image.repository().has_value()) {
        std::cerr << "Missing attribute 'repository' in ContainerImage: " << image.toString() << std::endl;
        return tags;
    }

    constexpr std::string_view endpoint{"/v2/namespaces/{}/repositories/{}/tags?page={}&page_size=100"};
    int page = 1;
    nlohmann::json json{};

    do {
        auto json_res = getJSON(std::format(endpoint,
                                            image.cnamespace().value(),
                                            image.repository().value(),
                                            page++));

        if (!json_res.has_value()) {
            std::cerr << json_res.error() << std::endl;
            return tags;
        }

        json = json_res.value();

        for (const auto &tag : json["results"]) {
            if (tag["name"].is_string()) {
                tags.insert(tag["name"]);
            }
        }

    } while (!json["next"].is_null());

    if (json["count"].is_number_integer()) {
        int expected = json["count"];
        if (expected == tags.size()) {
            std::cerr << std::format("Found all available tags ({}/{})\n", expected, tags.size());
        } else {
            std::cerr << std::format("Could not find all available tags ({}/{})\n", expected, tags.size());
        }
    }

    return tags;
}
