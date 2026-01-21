#pragma once

#include <string>
#include <optional>


class CommandLineInterface {
private:
    std::optional<std::string> m_config_file{};
    std::optional<std::string> m_container_engine_host{};
    
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
    const std::optional<std::string>& configFile() const { return m_config_file; }
    const std::optional<std::string>& containerEngineHost() const { return m_container_engine_host; }
};
