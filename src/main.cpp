#include "abstract_registry.hpp"
#include "cli.hpp"
#include "container_engine.hpp"
#include "container_image.hpp"
#include "docker_hub_registry.hpp"

#include <iostream>

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

        for (const auto &tag : registry->fetchTags(image)) {
            std::cout << "Found tag: " << tag << std::endl;
        }
    }

    return 0;
}
