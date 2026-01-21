#include "tag.hpp"

#include <string>
#include <string_view>
#include <regex>
#include <compare>

Tag::Tag(std::string_view tag) {
    m_raw = tag;

    std::regex number{"\\d+"};
    auto version_begin = std::sregex_iterator(m_raw.begin(), m_raw.end(), number);
    auto version_end = std::sregex_iterator();

    for (std::sregex_iterator i = version_begin; i != version_end; i++) {
        std::smatch match = *i;
        
        // should always be int-parsable due to regex
        m_version_components.push_back(std::stoi(match.str()));
    }
}

std::strong_ordering Tag::operator <=>(const Tag &rhs) const {
    if (!versionComponents().empty() && !rhs.versionComponents().empty()) {
        return versionComponents() <=> rhs.versionComponents();
    }

    if (!versionComponents().empty()) {
        return std::strong_ordering::greater;
    }

    if (!rhs.versionComponents().empty()) {
        return std::strong_ordering::less;
    }

    return raw() <=> rhs.raw();
}
