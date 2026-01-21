#pragma once

#include "cli.hpp"

#include <string>
#include <string_view>

class Config {
private:
    // how to connect to the container engine
    std::string m_container_engine_host{"unix:///run/user/1000/podman/podman.sock"};

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
    const std::string& containerEngineHost() const {return m_container_engine_host;}
};
