#pragma once
#include "raylib.h"
#include <string>

struct EngineMaterial {
    std::string name;
    Color albedoColor;
    Texture2D albedoTexture;
    std::string albedoPath;
    float roughness;
    float metallic;
    bool isLoaded;
    unsigned int id;

    EngineMaterial()
        : id(0),
          name("Default"),
          albedoColor(WHITE),
          albedoTexture({0}),
          roughness(0.5f),
          metallic(0.0f),
          isLoaded(false) {}

    bool LoadTextureFromFile(const std::string& path) {
        if (isLoaded) UnloadTexture(albedoTexture);

        Image image = LoadImage(path.c_str());
        if (image.data) {
            albedoTexture = LoadTextureFromImage(image);
            UnloadImage(image);
            isLoaded = true;
            return true;
        } else {
            Image fallback = GenImageColor(1, 1, albedoColor);
            albedoTexture = LoadTextureFromImage(fallback);
            UnloadImage(fallback);
            isLoaded = true;
            std::cout << "WARNING: Texture not found, using fallback color.\n";
            return false;
        }
    }


    void Unload() {
        if (isLoaded) {
            UnloadTexture(albedoTexture);
            isLoaded = false;
        }
    }
};
