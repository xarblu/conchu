#include "tag_filter.hpp"
#include "tag.hpp"

#include <algorithm>
#include <optional>
#include <regex>
#include <string>
#include <unordered_set>

TagFilter::TagFilter(const std::unordered_set<std::string> &tags, std::optional<std::regex> pattern) {
    for (const auto &tag : tags) {
        // no pattern filter
        if (!pattern.has_value()) {
            m_tags.push_back(Tag{tag});
            continue;
        }

        // pattern filter match
        if (std::regex_match(tag, pattern.value())) {
            m_tags.push_back(Tag{tag});
            continue;
        }
    }
}

std::optional<Tag> TagFilter::latest() const {
    if (m_tags.empty()) {
        return std::optional<Tag>{};
    }

    return *std::max_element(m_tags.begin(), m_tags.end());
}
