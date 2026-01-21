#pragma once

#include <string>
#include <string_view>
#include <vector>


class Tag {
private:
    // raw version of the tag, as passed to constructor
    std::string m_raw{};

    // numeric version components
    std::vector<int> m_version_components{};

public:
    /**
     * Parse tag string
     */
    explicit Tag(std::string_view tag);

    /**
     * Getters
     */
    const std::string& raw() const {return m_raw;};
    const std::vector<int>& versionComponents() const {return m_version_components;};

    /**
     * Compare version with other
     *
     * This uses numeric version components if available for both,
     * then falls back to basic string comparison.
     *
     * If only one side has numeric version components it's always assumed
     * to be larger.
     */
    std::strong_ordering operator <=>(const Tag &rhs) const;
};
