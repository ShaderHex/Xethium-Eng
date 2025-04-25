// SceneManager.cpp
#include "SceneManager.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

std::vector<RectangleObject> SceneManager::LoadScene(const std::string& filepath, Camera2D& camera) {
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

        if (elem.contains("name")) {
            std::string name = elem["name"];

            if (name == "cube") {
                // Cube object
                if (!elem.contains("x") || !elem.contains("width")) {
                    std::cerr << "[SceneManager] Skipping invalid cube entry in "
                              << filepath << "\n";
                    continue;
                }

                float x      = elem.value("x", 0.0f);
                float y      = elem.value("y", 0.0f);
                float w      = elem.value("width", 0.0f);
                float h      = elem.value("height", 0.0f);
                auto  colArr = elem.value("color", std::vector<int>{255, 255, 255, 255});
                Color color  = {
                    (unsigned char)colArr[0],
                    (unsigned char)colArr[1],
                    (unsigned char)colArr[2],
                    (unsigned char)colArr[3]
                };

                loaded.emplace_back(x, y, w, h, color);
            } else if (name == "camera") {
                // Camera object
                if (!elem.contains("target") || !elem.contains("offset")) {
                    std::cerr << "[SceneManager] Skipping invalid camera entry in "
                              << filepath << "\n";
                    continue;
                }

                Vector2 target = {
                    elem["target"][0].get<float>(), 
                    elem["target"][1].get<float>()
                };
                Vector2 offset = {
                    elem["offset"][0].get<float>(), 
                    elem["offset"][1].get<float>()
                };
                float rotation = elem.value("rotation", 0.0f);
                float zoom     = elem.value("zoom", 1.0f);

                // You can save the camera into a separate container or initialize it directly
                // For example, if you want to set the camera's initial values:
                camera.target = target;
                camera.offset = offset;
                camera.rotation = rotation;
                camera.zoom = zoom;

                std::cout << "[SceneManager] Camera loaded successfully\n";
            }
        }
    }

    return loaded;
}
