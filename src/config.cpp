#include "config.hpp"
#include "cli.hpp"

#include <string_view>

Config::Config(std::string_view file) {
    // TODO: parse a config file (yaml?)
}

void Config::mergeCLI(const CommandLineInterface &cli) {
    if (cli.containerEngineHost().has_value()) {
        m_containerEngineHost = cli.containerEngineHost().value();
    }
}
