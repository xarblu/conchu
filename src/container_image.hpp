#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <string_view>
#include <optional>

class ContainerImage {
private:
    // parts of a container image source like
    // <registry>/<namespace>/<repository>:<tag>
    std::optional<std::string> m_registry{};
    std::optional<std::string> m_namespace{};
    std::optional<std::string> m_repository{};
    std::optional<std::string> m_tag{};

public:
    /**
     * Empty container image
     */
    explicit ContainerImage() {}

    /**
     * Parse a container image source
     */
    explicit ContainerImage(std::string_view image);

    /**
     * Compare this ContainerImage to other
     */
    bool operator==(const ContainerImage& other) const;

    /**
     * Getters
     */
    std::optional<std::string> registry() const {return m_registry;}
    std::optional<std::string> cnamespace() const {return m_namespace;}
    std::optional<std::string> repository() const {return m_repository;}
    std::optional<std::string> tag() const {return m_tag;}

    /**
     * Check if parsed image is valid
     */
    bool isValid() const;

    /**
     * Convert to human readable string
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
    void printParseError(std::string_view image, std::string_view msg) const;
};
