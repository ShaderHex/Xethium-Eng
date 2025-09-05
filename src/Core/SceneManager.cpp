#include "SceneManager.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

void SceneManager::CreateScene(char* SceneName) {
    std::string sceneDir = "project/scenes/";
    fs::create_directories(sceneDir);

    std::ofstream file(sceneDir + std::string(SceneName) + ".json");

    if (file.is_open()) {
        file << "[]";
        file.close();
        std::cout << "[SceneManager] Scene created successfully: " << SceneName << "\n";
    } else {
        std::cerr << "[SceneManager] Failed to create the file.\n";
    }
}

std::vector<RectangleObject> SceneManager::LoadScene(const std::string& filepath, Camera3D& editorCamera, Camera3D& playCamera) {
    std::vector<RectangleObject> rectangles;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[SceneManager] Error: Could not open file " << filepath << "\n";
        return rectangles;
    }

    if (file.peek() == std::ifstream::traits_type::eof()) {
        std::cerr << "[SceneManager] Error: Scene file is empty (" << filepath << ")\n";
        return rectangles;
    }

    json sceneJson;
    try {
        file >> sceneJson;
    } catch (const json::parse_error& e) {
        std::cerr << "[SceneManager] Error: Malformed JSON in " << filepath << "\n";
        std::cerr << "Parser message: " << e.what() << "\n";
        return rectangles;
    }

    if (!sceneJson.is_array()) {
        std::cerr << "[SceneManager] Error: Invalid scene format - root must be an array\n";
        return rectangles;
    }

    for (auto& entry : sceneJson) {
        if (!entry.is_object()) continue;

        if (!entry.contains("name")) {
            std::cerr << "[SceneManager] Warning: object missing 'name' field\n";
            continue;
        }

        std::string name = entry["name"];

        if (name == "EditorCamera") {
            try {
                editorCamera.position = {
                    entry["position"][0],
                    entry["position"][1],
                    entry["position"][2]
                };
                editorCamera.target = {
                    entry["target"][0],
                    entry["target"][1],
                    entry["target"][2]
                };
                editorCamera.up = {
                    entry["up"][0],
                    entry["up"][1],
                    entry["up"][2]
                };
                editorCamera.fovy = entry["fovy"];
                editorCamera.projection = entry["projection"];
            } catch (const json::exception& e) {
                std::cerr << "[SceneManager] Error loading EditorCamera: " << e.what() << "\n";
            }
        } else if (name == "PlayCamera") {
            try {
                playCamera.position = {
                    entry["position"][0],
                    entry["position"][1],
                    entry["position"][2]
                };
                playCamera.target = {
                    entry["target"][0],
                    entry["target"][1],
                    entry["target"][2]
                };
                playCamera.up = {
                    entry["up"][0],
                    entry["up"][1],
                    entry["up"][2]
                };
                playCamera.fovy = entry["fovy"];
                playCamera.projection = entry["projection"];
            } catch (const json::exception& e) {
                std::cerr << "[SceneManager] Error loading PlayCamera: " << e.what() << "\n";
            }
        } else {
            RectangleObject rect;
            try {
                rect.name = name;
                rect.position.x = entry["x"];
                rect.position.y = entry["y"];
                rect.position.z = entry["z"];
                rect.size.x = entry["width"];
                rect.size.y = entry["height"];
                rect.size.z = entry["depth"];

                if (entry.contains("color") && entry["color"].is_array()) {
                    auto& color = entry["color"];
                    rect.color.r = color[0].get<uint8_t>();
                    rect.color.g = color[1].get<uint8_t>();
                    rect.color.b = color[2].get<uint8_t>();
                    rect.color.a = color.size() > 3 ? color[3].get<uint8_t>() : 255;
                }

                rect.UiD = entry["uid"];
                currentUiD = rect.UiD + 1;
                rectangles.push_back(rect);
            } catch (const json::exception& e) {
                std::cerr << "[SceneManager] Invalid rectangle: " << e.what() << "\n";
            }
        }
    }

    std::cout << "[SceneManager] Scene loaded with " << rectangles.size() << " rectangles\n";
    return rectangles;
}

void SceneManager::SaveScene(const std::vector<RectangleObject>& rectangles, const Camera3D& editorCamera, const Camera3D& playCamera, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[SceneManager] Failed to open file for writing!\n";
        return;
    }

    file << "[\n";

    auto writeCam = [&](const std::string& name, const Camera3D& cam) {
        file << "  {\n";
        file << "    \"name\": \"" << name << "\",\n";
        file << "    \"position\": [" << cam.position.x << ", " << cam.position.y << ", " << cam.position.z << "],\n";
        file << "    \"target\": [" << cam.target.x << ", " << cam.target.y << ", " << cam.target.z << "],\n";
        file << "    \"up\": [" << cam.up.x << ", " << cam.up.y << ", " << cam.up.z << "],\n";
        file << "    \"fovy\": " << cam.fovy << ",\n";
        file << "    \"projection\": " << cam.projection << "\n";
        file << "  },\n";
    };

    writeCam("EditorCamera", editorCamera);
    writeCam("PlayCamera", playCamera);

    for (size_t i = 0; i < rectangles.size(); ++i) {
        const RectangleObject& rect = rectangles[i];
        file << "  {\n";
        file << "    \"name\": \"" << rect.name << "\",\n";
        file << "    \"x\": " << rect.position.x << ",\n";
        file << "    \"y\": " << rect.position.y << ",\n";
        file << "    \"z\": " << rect.position.z << ",\n";
        file << "    \"width\": " << rect.size.x << ",\n";
        file << "    \"height\": " << rect.size.y << ",\n";
        file << "    \"depth\": " << rect.size.z << ",\n";
        file << "    \"color\": [" << (int)rect.color.r << ", " << (int)rect.color.g << ", " << (int)rect.color.b << ", " << (int)rect.color.a << "],\n";
        file << "    \"uid\": " << rect.UiD << "\n";
        file << "  }";
        if (i < rectangles.size() - 1) file << ",\n";
    }

    file << "\n]\n";
    file.close();
    std::cout << "[SceneManager] Scene saved to " << filename << "\n";
}
