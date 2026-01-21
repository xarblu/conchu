#pragma once

#include "container.hpp"

#include <httplib.h>

#include <string>
#include <memory>
#include <vector>

class ContainerEngine {
private:
    std::unique_ptr<httplib::Client> m_client;

public:
    /**
     * Construct ContainerEngine from a host URL
     * Supported schemes:
     *  unix:// - uses UNIX domain sockets
     *  http(s):// - uses HTTP(S)
     */
    explicit ContainerEngine(std::string host);

    /**
     * Check if the instance is valid
     */
    bool isValid() const;

    /**
     * Get a list of containers with relevant metadata
     */
    std::vector<Container> getContainers() const;
};
