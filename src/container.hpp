#pragma once

#include "container_image.hpp"

#include <nlohmann/json.hpp>

#include <string>
#include <string_view>
#include <optional>
#include <unordered_map>

class Container {
private:
    std::optional<std::string> m_name{};
    std::optional<ContainerImage> m_image{};
    std::unordered_map<std::string, std::string> m_labels{};

public:
    /**
     * Parse container metadata from JSON object
     */
    explicit Container(nlohmann::json &json);

    /**
     * Convert Container to human readable string
     */
    std::string toString() const;

private:
    /**
     * Print a parse error
     */
    void printParseError(nlohmann::json &json, std::string_view msg) const;
};
