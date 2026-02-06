#include "cli.hpp"
#include "config.hpp"
#include "enums.hpp"
#include "mode_oneshot.hpp"
#include "mode_daemon.hpp"

#include <memory>

int main(int argc, char *argv[]) {
    CommandLineInterface cli{argc, argv};
    auto config = std::make_shared<Config>(cli.configFile().value_or("./conchu.yaml"));
    config->mergeCLI(cli);

    switch (cli.mode()) {
        case Mode::Oneshot:
            return oneshot_init(config);
        case Mode::Daemon:
            return daemon_init(config);
    }
}
