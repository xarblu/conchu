#pragma once

#include "tag.hpp"

#include <string>
#include <optional>
#include <regex>
#include <unordered_set>
#include <vector>

class TagFilter {
private:
    std::vector<Tag> m_tags{};

public:
    /**
     * Construct a TagFilter, optionally filtering tags by
     * a regex pattern
     */
    explicit TagFilter(const std::unordered_set<std::string> &tags, std::optional<std::regex> pattern = std::optional<std::regex>{});


    /**
     * Get latest tag, empty if no tags in m_tags
     */
    std::optional<Tag> latest() const;
};
