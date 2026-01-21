#include "abstract_registry.hpp"
#include "cli.hpp"
#include "container_engine.hpp"
#include "container_image.hpp"
#include "docker_hub_registry.hpp"
#include "tag_filter.hpp"

#include <iostream>
#include <regex>

int main(int argc, char *argv[]) {
    CommandLineInterface cli{argc, argv};

    ContainerEngine engine{cli.containerEngineHost().value_or("unix:///run/user/1000/podman/podman.sock")};
    DockerHubRegistry docker_hub{};

    for (const auto &container : engine.getContainers()) {
        std::cout << "Found Container: " << container.toString() << std::endl;

        if (!container.image().has_value()) {
            std::cout << "Skipping due to empty image attribute" << std::endl;
            continue;
        }

        ContainerImage image{container.image().value()};
        AbstractRegistry* registry{nullptr};

        if (docker_hub.matchContainerImage(image)) {
            std::cout << "Matched registry Docker Hub" << std::endl;
            registry = &docker_hub;
        }

        if (!registry){
            std::cout << "No registry matched" << std::endl;
            continue;
        }

        auto tags = registry->fetchTags(image);

        TagFilter tag_filter{tags, std::regex{"\\d+\\.\\d+\\.\\d+"}};
        auto latest_tag = tag_filter.latest();
        if (latest_tag.has_value()) {
            std::cout << "Latest tag: " << latest_tag.value().raw() << std::endl;
        } else {
            std::cout << "Could not find latest tag - No tags?" << std::endl;
        }

    }

    return 0;
}
