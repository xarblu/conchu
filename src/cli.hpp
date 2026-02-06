#pragma once

#include "enums.hpp"

#include <string>
#include <optional>


class CommandLineInterface {
private:
    // CLI only
    std::optional<std::string> m_configFile{};
    std::optional<Mode> m_mode{};

    // overrides for config file
    std::optional<std::string> m_containerEngineHost{};
    
public:
    /**
     * Parse CLI arguments
     * @param argc  argc forwarded from main()
     * @param argv  argv forwarded from main()
     */
    explicit CommandLineInterface(int argc, char *argv[]);

    /**
     * Print usage message
     */
    void usage() const;

    /**
     * Getters
     */
    const std::optional<std::string>& configFile() const { return m_configFile; }
    const std::optional<std::string>& containerEngineHost() const { return m_containerEngineHost; }
};
