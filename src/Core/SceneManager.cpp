#include "SceneManager.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

std::vector<RectangleObject> SceneManager::LoadScene(const std::string& filepath, Camera2D& editorCamera, Camera2D& playCamera)
{
    std::vector<RectangleObject> rectangles;

    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "[SceneManager] Error: Could not open file " << filepath << "\n";
        return rectangles;
    }

    if (file.peek() == std::ifstream::traits_type::eof())
    {
        std::cerr << "[SceneManager] Error: Scene file is empty (" << filepath << ")\n";
        return rectangles;
    }

    json sceneJson;
    try {
        file >> sceneJson;
    }
    catch (const json::parse_error& e) {
        std::cerr << "[SceneManager] Error: Malformed JSON in " << filepath << "\n";
        std::cerr << "Parser message: " << e.what() << "\n";
        return rectangles;
    }

    if (!sceneJson.is_array()) {
        std::cerr << "[SceneManager] Error: Invalid scene format - root must be an array\n";
        return rectangles;
    }

    for (auto& entry : sceneJson)
    {
        if (!entry.is_object()) {
            std::cerr << "[SceneManager] Warning: Skipping invalid array entry\n";
            continue;
        }

        if (!entry.contains("name")) {
            std::cerr << "[SceneManager] Warning: Entry missing 'name' field\n";
            continue;
        }

        std::string name = entry["name"];
        
        if (name == "EditorCamera") {
            try {
                editorCamera.target.x = entry["target"][0];
                editorCamera.target.y = entry["target"][1];
                editorCamera.offset.x = entry["offset"][0];
                editorCamera.offset.y = entry["offset"][1];
                editorCamera.rotation = entry["rotation"];
                editorCamera.zoom = entry["zoom"];
            }
            catch (const json::exception& e) {
                std::cerr << "[SceneManager] Error: Invalid EditorCamera data - " << e.what() << "\n";
            }
        }
        else if (name == "PlayCamera") {
            try {
                playCamera.target.x = entry["target"][0];
                playCamera.target.y = entry["target"][1];
                playCamera.offset.x = entry["offset"][0];
                playCamera.offset.y = entry["offset"][1];
                playCamera.rotation = entry["rotation"];
                playCamera.zoom = entry["zoom"];
            }
            catch (const json::exception& e) {
                std::cerr << "[SceneManager] Error: Invalid PlayCamera data - " << e.what() << "\n";
            }
        }
        else {
            RectangleObject rect;
            try {
                rect.name = name;
                rect.position.x = entry["x"];
                rect.position.y = entry["y"];
                rect.size.x = entry["width"];
                rect.size.y = entry["height"];

                if (entry.contains("color") && entry["color"].is_array()) {
                    auto& color = entry["color"];
                    rect.color.r = color[0].get<uint8_t>();
                    rect.color.g = color[1].get<uint8_t>();
                    rect.color.b = color[2].get<uint8_t>();
                    rect.color.a = color.size() > 3 ? color[3].get<uint8_t>() : 255;
                }
                rectangles.push_back(rect);
            }
            catch (const json::exception& e) {
                std::cerr << "[SceneManager] Error: Invalid rectangle data for '" 
                         << name << "' - " << e.what() << "\n";
            }
        }
    }

    std::cout << "[SceneManager] Successfully loaded scene with: \n"
              << "  - " << rectangles.size() << " rectangles\n"
              << "  - Editor Camera: " << (sceneJson[0].contains("EditorCamera") ? "Loaded" : "Missing") << "\n"
              << "  - Play Camera: " << (sceneJson[1].contains("PlayCamera") ? "Loaded" : "Missing") << "\n";

    return rectangles;
}

void SceneManager::SaveScene(
    const std::vector<RectangleObject>& rectangles, 
    const Camera2D& editorCamera, 
    const Camera2D& playCamera, 
    const std::string& filename
) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[SceneManager] Failed to open file for writing!\n";
        return;
    }

    file << "[\n";

    file << "  {\n";
    file << "    \"name\": \"EditorCamera\",\n";
    file << "    \"target\": [" << editorCamera.target.x << ", " << editorCamera.target.y << "],\n";
    file << "    \"offset\": [" << editorCamera.offset.x << ", " << editorCamera.offset.y << "],\n";
    file << "    \"rotation\": " << editorCamera.rotation << ",\n";
    file << "    \"zoom\": " << editorCamera.zoom << "\n";
    file << "  },\n";

    file << "  {\n";
    file << "    \"name\": \"PlayCamera\",\n";
    file << "    \"target\": [" << playCamera.target.x << ", " << playCamera.target.y << "],\n";
    file << "    \"offset\": [" << playCamera.offset.x << ", " << playCamera.offset.y << "],\n";
    file << "    \"rotation\": " << playCamera.rotation << ",\n";
    file << "    \"zoom\": " << playCamera.zoom << "\n";
    file << "  }";

    for (size_t i = 0; i < rectangles.size(); ++i) {
        const RectangleObject& rect = rectangles[i];

        file << ",\n";
        file << "  {\n";
        file << "    \"name\": \"" << rect.name << "\",\n";
        file << "    \"x\": " << rect.position.x << ",\n";
        file << "    \"y\": " << rect.position.y << ",\n";
        file << "    \"width\": " << rect.size.x << ",\n";
        file << "    \"height\": " << rect.size.y << ",\n";
        file << "    \"color\": [" 
             << (int)rect.color.r << ", "
             << (int)rect.color.g << ", "
             << (int)rect.color.b << ", "
             << (int)rect.color.a << "]\n";
        file << "  }";
    }

    file << "\n]\n";

    file.close();
    std::cout << "[SceneManager] Scene saved successfully to " << filename << "!\n";
}
