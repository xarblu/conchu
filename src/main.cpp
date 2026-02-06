#include "cli.hpp"
#include "config.hpp"
#include "enums.hpp"
#include "mode_oneshot.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    CommandLineInterface cli{argc, argv};
    Config config{cli.configFile().value_or("./conchu.yaml")};
    config.mergeCLI(cli);

    switch (cli.mode()) {
        case Mode::Oneshot:
            return oneshot_init(config);
        case Mode::Daemon:
            std::cerr << "Not implemented\n";
            return 1;
    }
}
