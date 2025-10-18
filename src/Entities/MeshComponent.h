#pragma once

#include <string>
#include <vector>
#include "raylib.h"
#include "Core/UiDgenerator.h"

struct MeshObject {
    unsigned int UiD;
    std::string name;
    std::string modelPath;
    bool enableBloom = false;
    Model model{};
    bool isLoaded = false;

    Vector3 position{0,0,0};
    Vector3 rotation{0,0,0};
    Vector3 scale{1.0f,1.0f,1.0f};
    Color color = WHITE;

    MeshObject() = default;
    MeshObject(const std::string& path, const Vector3& pos)
        : modelPath(path), position(pos), UiD(UiDGenerator::GenerateUiD()) {
        name = "Mesh" + std::to_string(UiD);
    }

void Load() {
    if (isLoaded) Unload();
    model = LoadModel(modelPath.c_str());
    isLoaded = true;
}


    void Reload() {
        Load();
    }

    void Unload() {
        if (isLoaded) {
            UnloadModel(model);
            isLoaded = false;
        }
    }
};
