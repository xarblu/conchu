#pragma once

#include <string>
#include <string_view>
#include <unordered_set>


class AbstractRegistry {
private:
public:
    /**
     * Whether this registry matches the image source string
     * @param source  Image source e.g. docker.io/library/ubuntu:24.04
     */
    virtual bool matchSource(std::string_view source) const = 0;

    /**
     * Fetch tags for source from registry
     */
    virtual std::unordered_set<std::string> fetchTags(std::string_view source) = 0;
};
