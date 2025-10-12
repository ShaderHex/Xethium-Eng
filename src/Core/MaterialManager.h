#pragma once
#include <unordered_map>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"
#include "raylib.h"
#include "Core/UiDgenerator.h"

using json = nlohmann::json;

class MaterialManager {
private:
    std::unordered_map<unsigned int, EngineMaterial> materials;

public:
    EngineMaterial* CreateMaterial(const std::string& name, const std::string& texturePath, Color color) {
        EngineMaterial mat;
        mat.id = UiDGenerator::GenerateUiD();
        mat.name = name;
        mat.albedoColor = color;
        mat.albedoPath = texturePath;
        mat.roughness = 0.5f;
        mat.metallic = 0.0f;
        mat.isLoaded = true;
        Image img = LoadImage(texturePath.c_str());
        mat.albedoTexture = LoadTextureFromImage(img);
        UnloadImage(img);

        materials[mat.id] = mat;

        // Save to .mat file automatically
        SaveToFile(mat);

        return &materials[mat.id];
    }

    void SaveToFile(const EngineMaterial& mat) {
        json j;
        j["name"] = mat.name;
        j["albedoColor"] = {mat.albedoColor.r, mat.albedoColor.g, mat.albedoColor.b, mat.albedoColor.a};
        j["albedoTexture"] = mat.albedoPath;
        j["roughness"] = mat.roughness;
        j["metallic"] = mat.metallic;

        std::ofstream file("project/assets/materials/" + mat.name + ".mat");
        if (file.is_open()) {
            file << j.dump(4);
            file.close();
        }
    }

    void LoadMaterialFromFile(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) return;

        json j;
        file >> j;
        file.close();

        EngineMaterial mat;
        mat.id = UiDGenerator::GenerateUiD();
        mat.name = j["name"];
        mat.albedoColor = {
            (unsigned char)j["albedoColor"][0],
            (unsigned char)j["albedoColor"][1],
            (unsigned char)j["albedoColor"][2],
            (unsigned char)j["albedoColor"][3]
        };
        mat.albedoPath = j["albedoTexture"];
        mat.roughness = j["roughness"];
        mat.metallic = j["metallic"];

        Image img = LoadImage(mat.albedoPath.c_str());
        mat.albedoTexture = LoadTextureFromImage(img);
        UnloadImage(img);
        mat.isLoaded = true;

        materials[mat.id] = mat;
    }

    EngineMaterial* GetById(unsigned int id) {
        if (materials.find(id) != materials.end())
            return &materials[id];
        return nullptr;
    }

    EngineMaterial* GetByName(const std::string& name) {
        for (auto& [id, mat] : materials) {
            if (mat.name == name) return &mat;
        }
        return nullptr;
    }

};
