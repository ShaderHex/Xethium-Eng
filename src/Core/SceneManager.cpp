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

SceneData SceneManager::LoadScene(const std::string& filepath, Camera3D& editorCamera, Camera3D& playCamera) {
    SceneData data;
    
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[SceneManager] Error: Could not open file " << filepath << "\n";
        return data;
    }

    if (file.peek() == std::ifstream::traits_type::eof()) {
        std::cerr << "[SceneManager] Error: Scene file is empty (" << filepath << ")\n";
        return data;
    }

    json sceneJson;
    try {
        file >> sceneJson;
    } catch (const json::parse_error& e) {
        std::cerr << "[SceneManager] Error: Malformed JSON in " << filepath << "\n";
        std::cerr << "Parser message: " << e.what() << "\n";
        return data;
    }

    if (!sceneJson.is_array()) {
        std::cerr << "[SceneManager] Error: Invalid scene format - root must be an array\n";
        return data;
    }

    for (auto& entry : sceneJson) {
        if (!entry.is_object()) continue;

        std::string type = entry.value("type", "");

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
        } else if (type == "Rectangle") {
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
                data.rectangles.push_back(rect);
            } catch (const json::exception& e) {
                std::cerr << "[SceneManager] Invalid rectangle: " << e.what() << "\n";
            }
        } else if (type == "Light") {
            LightEntity light;
            try {
                light.name = name;
                light.position.x = entry["x"];
                light.position.y = entry["y"];
                light.position.z = entry["z"];
                light.intensity = entry["intensity"];
                if (entry.contains("color") && entry["color"].is_array()) {
                    auto& color = entry["color"];
                    light.color.r = color[0].get<uint8_t>();
                    light.color.g = color[1].get<uint8_t>();
                    light.color.b = color[2].get<uint8_t>();
                    light.color.a = color.size() > 3 ? color[3].get<uint8_t>() : 255;
                }

                light.UiD = entry["uid"];
                currentUiD = light.UiD + 1;
                data.lights.push_back(light);
            } catch (const json::exception& e) {
                std::cerr << "[SceneManager] Invalid light: " << e.what() << "\n";
            }
        }
    }

    std::cout << "[SceneManager] Scene loaded with " << data.rectangles.size() << " rectangles and " << data.lights.size() << " lights\n";
    return data;
}

void SceneManager::SaveScene(const std::vector<RectangleObject>& rectangles,
                             const std::vector<LightEntity>& lights,
                             const Camera3D& editorCamera,
                             const Camera3D& playCamera,
                             const std::string& filename) 
{
    json sceneJson = json::array();

    auto camToJson = [](const std::string& name, const Camera3D& cam) {
        return json{
            {"name", name},
            {"position", {cam.position.x, cam.position.y, cam.position.z}},
            {"target",   {cam.target.x,   cam.target.y,   cam.target.z}},
            {"up",       {cam.up.x,       cam.up.y,       cam.up.z}},
            {"fovy", cam.fovy},
            {"projection", cam.projection}
        };
    };

    sceneJson.push_back(camToJson("EditorCamera", editorCamera));
    sceneJson.push_back(camToJson("PlayCamera", playCamera));

    for (const auto& rect : rectangles) {
        sceneJson.push_back({
            {"type", "Rectangle"},
            {"name", rect.name},
            {"x", rect.position.x},
            {"y", rect.position.y},
            {"z", rect.position.z},
            {"width", rect.size.x},
            {"height", rect.size.y},
            {"depth", rect.size.z},
            {"color", {rect.color.r, rect.color.g, rect.color.b, rect.color.a}},
            {"uid", rect.UiD}
        });
    }

    for (const auto& light : lights) {
        sceneJson.push_back({
            {"type", "Light"},
            {"name", light.name},
            {"x", light.position.x},
            {"y", light.position.y},
            {"z", light.position.z},
            {"intensity", light.intensity},
            {"color", {light.color.r, light.color.g, light.color.b, light.color.a}},
            {"uid", light.UiD}
        });
    }

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[SceneManager] Failed to open file for writing!\n";
        return;
    }

    file << sceneJson.dump(4);
    file.close();

    std::cout << "[SceneManager] Scene saved to " << filename << "\n";
}