#include "cli.hpp"

#include <string_view>
#include <iostream>
#include <cstdlib>


CommandLineInterface::CommandLineInterface(int argc, char *argv[]) {
    for (int i{1}; i < argc; i++) {
        std::basic_string_view arg{argv[i]};

        if (arg == "-h" || arg == "--help") {
            usage();
            std::exit(0);
        }

        if (arg == "-c" || arg == "--config") {
            if (i + 1 < argc) {
                m_configFile = std::string{argv[++i]};
            } else {
                std::cerr << "Missing value for: " << arg << "\n\n";
                usage();
                std::exit(1);
            }
            continue;
        }

        if (arg == "-e" || arg == "--engine-host") {
            if (i + 1 < argc) {
                m_containerEngineHost = std::string{argv[++i]};
            } else {
                std::cerr << "Missing value for: " << arg << "\n\n";
                usage();
                std::exit(1);
            }
            continue;
        }

        // positional argument
        if (!arg.starts_with("-")) {
            if (!m_mode.has_value()) {
                if (arg == "oneshot") {
                    m_mode = Mode::Oneshot;
                } else if (arg == "daemon") {
                    m_mode = Mode::Daemon;
                } else {
                    std::cerr << "Bad value for MODE: " << arg << "\n\n";
                    usage();
                    std::exit(1);
                }
            }
            continue;
        }

        std::cerr << "Unknown argument: " << arg << "\n\n";
        usage();
        std::exit(1);
    }

    if (!m_mode.has_value()) {
        std::cerr << "Missing required arg: MODE\n\n";
        usage();
        std::exit(1);
    }
}

void CommandLineInterface::usage() const {
    std::cout << "conchu - A simple container update checker\n"
              << "Usage: conchu [-h|--help] MODE\n"
              << "\n"
              << "Options:\n"
              << "  -c|--config       Config file (default ./conchu.yaml)\n"
              << "  -e|--engine-host  Container engine host to use\n"
              << "                    Should include scheme, e.g. unix:///run/podman/podman.sock\n"
              << "  -h|--help         Print help and exit\n"
              << "\n"
              << "Positional arguments:\n"
              << "  MODE              Mode to operate in:\n"
              << "                    oneshot - Check all containers once, print potential updates, then exit\n"
              << "                    daemon  - Continuously check for container updates in the configured interval\n";
}
