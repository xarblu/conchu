#pragma once

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
     * Parse a container image source
     */
    explicit ContainerImage(std::string_view image);

    /**
     * Check if parsed image is valid
     */
    bool isValid() const;

    /**
     * Convert to human readable string
     */
    std::string toString() const;

private:
    /**
     * Print a parse error
     */
    void printParseError(std::string_view image, std::string_view msg) const;
};
