// SceneManager.cpp
#include "SceneManager.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

std::vector<RectangleObject> SceneManager::LoadScene(const std::string& filepath) {
    std::vector<RectangleObject> loaded;

    if (!fs::exists(filepath)) {
        std::cerr << "[SceneManager] File not found: " << filepath
                  << " → Starting with empty scene\n";
        return loaded;
    }

    std::ifstream in(filepath);
    if (!in.is_open()) {
        std::cerr << "[SceneManager] Unable to open file: " << filepath << "\n";
        return loaded;
    }

    json j;
    try {
        in >> j;
    } catch (const json::parse_error& e) {
        std::cerr << "[SceneManager] JSON parse error in " << filepath
                  << ": " << e.what() << "\n";
        return loaded;
    }

    if (!j.is_array()) {
        std::cerr << "[SceneManager] Expected JSON array in "
                  << filepath << ", got " << j.type_name() << "\n";
        return loaded;
    }

    for (const auto& elem : j) {
        if (elem.is_null()) continue;

        if (!elem.contains("x") || !elem.contains("width")) {
            std::cerr << "[SceneManager] Skipping invalid entry in "
                      << filepath << "\n";
            continue;
        }

        float x      = elem.value("x", 0.0f);
        float y      = elem.value("y", 0.0f);
        float w      = elem.value("width",  0.0f);
        float h      = elem.value("height", 0.0f);
        auto  colArr = elem.value("color", std::vector<int>{255,255,255,255});
        Color color  = {
            (unsigned char)colArr[0],
            (unsigned char)colArr[1],
            (unsigned char)colArr[2],
            (unsigned char)colArr[3]
        };

        loaded.emplace_back(x, y, w, h, color);
    }

    return loaded;
}
