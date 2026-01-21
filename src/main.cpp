#include "cli.hpp"
#include "container_engine.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    CommandLineInterface cli{argc, argv};

    ContainerEngine engine{cli.containerEngineHost().value_or("unix:///run/user/1000/podman/podman.sock")};

    for (auto &container : engine.getContainers()) {
        std::cout << container.toString() << std::endl;
    }

    return 0;
}
