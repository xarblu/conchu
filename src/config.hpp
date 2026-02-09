#pragma once

#include "cli.hpp"

#include <string>
#include <string_view>
#include <chrono>
#include <cstdint>

class Config {
private:
    // how to connect to the container engine
    std::string m_containerEngineHost{"unix:///run/user/1000/podman/podman.sock"};

    // -- Daemon Options --
    // check interval in seconds (Daemon mode only)
    std::chrono::seconds m_checkInterval{3600};

    // -- Api Options --
    std::string m_listenAddr{"127.0.0.1"};
    std::uint32_t m_listenPort{6767};

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
    std::string containerEngineHost() const {return m_containerEngineHost;}
    std::chrono::seconds checkInterval() const {return m_checkInterval;}
    std::string listenAddr() const {return m_listenAddr;}
    std::uint32_t listenPort() const {return m_listenPort;}
};
