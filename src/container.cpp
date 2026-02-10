#include "container.hpp"
#include "container_image.hpp"

#include <nlohmann/json.hpp>

#include <iostream>
#include <ostream>
#include <format>


Container::Container(nlohmann::json &json) {
    if (!json.is_object()) {
        printParseError(json, "Can't parse Container from non-object JSON");
        return;
    }

    if (json.contains("Names")) {
        auto names_json = json["Names"];
        if (!names_json.is_array()) {
            printParseError(json, ".Names should be an array");
            return;
        }
        auto names = names_json.get<std::vector<std::string>>();
        if (names.empty()) {
            printParseError(json, ".Names array is empty");
            return;
        }
        // Podman adds a / in front of names
        auto name = names[0];
        if (name.starts_with("/")) {
            m_name = name.substr(1);
        } else {
            m_name = name;
        }
    } else {
        printParseError(json, ".Names field missing");
        return;
    }

    if (json.contains("Image")) {
        auto image = json["Image"];
        if (!image.is_string()) {
            printParseError(json, ".Image should be a string");
            return;
        }
        m_image = ContainerImage{image.get<std::string>()};
    } else {
        printParseError(json, ".Image field missing");
        return;
    }

    if (json.contains("Labels")) {
        auto labels = json["Labels"];
        if (!labels.is_object()) {
            printParseError(json, ".Labels should be a object");
            return;
        }
        m_labels = labels.get<std::unordered_map<std::string, std::string>>();
    } else {
        printParseError(json, ".Labels field missing");
        return;
    }
}

bool Container::operator==(const Container& other) const{
    return m_name == other.name() &&
           m_image == other.image() &&
           m_labels == other.labels();
}

void Container::printParseError(nlohmann::json &json, std::string_view msg) const {
    std::cerr << "Error parsing Container from JSON:\n"
              << json.dump(2)
              << "\n" << msg << std::endl;
}

std::string Container::toString() const {
    std::string labels{};
    for (const auto &[key, value] : m_labels) {
        if (!labels.empty()) {
            labels += " ";
        }
        labels += (std::format("{}={}", key, value));
    }

    return std::format("name={} image=({}) labels=({})",
                       m_name.value_or("Unknown"),
                       m_image.transform([](ContainerImage image) {return image.toString();}).value_or("Unknown"),
                       labels);
}

nlohmann::json Container::toJSON() const {
    nlohmann::json json{};

    json["name"] = m_name;
    json["image"] = m_image.transform([](ContainerImage image){return image.toJSON();}).value_or(nlohmann::json{});
    json["labels"] = m_labels;

    return json;
}
