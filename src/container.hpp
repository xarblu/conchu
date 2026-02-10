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
     * Compare this Container to other
     */
    bool operator==(const Container& other) const;

    /**
     * Getters
     */
    std::optional<std::string> name() const {return m_name;}
    std::optional<ContainerImage> image() const {return m_image;}
    std::unordered_map<std::string, std::string> labels() const {return m_labels;}

    /**
     * Convert Container to human readable string
     */
    std::string toString() const;

    /**
     * Convert Container to JSON
     */
    nlohmann::json toJSON() const;

private:
    /**
     * Print a parse error
     */
    void printParseError(nlohmann::json &json, std::string_view msg) const;
};
