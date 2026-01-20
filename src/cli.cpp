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
        } else {
            std::cout << "Bad argument: " << arg << "\n\n";
            usage();
            std::exit(1);
        }
    }
}

void CommandLineInterface::usage() const {
    std::cout << "conchu - A simple container update checker\n"
              << "Usage: conchu [-h|--help] MODE\n"
              << "\n"
              << "Options:\n"
              << "  -h|--help  Print help and exit\n"
              << "\n"
              << "Positional arguments:\n"
              << "  MODE       Mode to operate in:\n"
              << "             oneshot - Check all containers once, print potential updates, then exit\n";
}
