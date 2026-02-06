#pragma once

#include "cli.hpp"

#include <string>
#include <string_view>
#include <chrono>

class Config {
private:
    // how to connect to the container engine
    std::string m_containerEngineHost{"unix:///run/user/1000/podman/podman.sock"};

    // check interval in seconds (Daemon mode only)
    std::chrono::seconds m_checkInterval{3600};

public:
    /**
     * Construct Config from config file
     */
    explicit Config(std::string_view file);

    /**
     * Merge CLI overrides
     */
    void mergeCLI(const CommandLineInterface &cli);

    /**
     * Getters
     */
    const std::string& containerEngineHost() const {return m_containerEngineHost;}
    const std::chrono::seconds& checkInterval() const {return m_checkInterval;}
};
