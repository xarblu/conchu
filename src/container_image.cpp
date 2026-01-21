#include "container_image.hpp"

#include <iomanip>
#include <iostream>
#include <ranges>
#include <string_view>
#include <vector>
#include <format>


ContainerImage::ContainerImage(std::string_view image) {
    // we expect <registry>/<namespace>/<repository>:<tag>

    auto parts = std::ranges::to<std::vector<std::string>>(std::views::split(image, '/'));
    if (parts.size() != 3) {
        printParseError(image, std::format("Unexpected amount of /-seperated components. Expected 3, got {}.", parts.size()));
        return;
    }

    m_registry = parts[0];
    m_namespace = parts[1];

    auto repo_parts = std::ranges::to<std::vector<std::string>>(std::views::split(parts[2], ':'));
    if (repo_parts.size() != 2) {
        printParseError(image, std::format("Unexpected amount of :-seperated components. Expected 2, got {}.", repo_parts.size()));
        return;
    }

    m_repository = repo_parts[0];
    m_tag = repo_parts[1];
}

void ContainerImage::printParseError(std::string_view image, std::string_view msg) const {
    std::cerr << "Failed to parse image source "
              << std::quoted(image)
              << "\n" << msg << "\n";
}

bool ContainerImage::isValid() const {
    return m_registry.has_value() &&
           m_namespace.has_value() &&
           m_repository.has_value() &&
           m_tag.has_value();
}

std::string ContainerImage::toString() const {
    return std::format("registry={} namespace={} repository={} tag={}",
                       m_registry.value_or("Unknown"),
                       m_namespace.value_or("Unknown"),
                       m_repository.value_or("Unknown"),
                       m_tag.value_or("Unknown"));
}
