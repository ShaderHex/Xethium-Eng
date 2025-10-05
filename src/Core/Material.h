#pragma once
#include "raylib.h"
struct EngineMaterial {
    Color albedoColor;
    Texture2D albedoTexture;
    float roughness;
    float metallic;
    bool isLoaded = false;

    EngineMaterial() {
        albedoColor = WHITE;
        albedoTexture = {0};
        roughness = 0.5f;
        metallic = 0.0f;
        
        Image img = GenImageColor(1, 1, albedoColor);
        albedoTexture = LoadTextureFromImage(img);
        UnloadImage(img);
    }

    void SetAlbedo(Color color) {
        albedoColor = color;
    }

    void SetTexture(const char* imageDir) {
        Image image = LoadImage(imageDir);
        Texture2D texture = LoadTextureFromImage(image);
        UnloadImage(image);

        if (texture.id > 0)
        {
            printf("Texture loaded\n");
            albedoTexture = texture;
        }
        else
        {
            printf("Failed to load texture\n");
            Image img = GenImageColor(1, 1, albedoColor);
            Texture2D tex = LoadTextureFromImage(img);
            UnloadImage(img);
            albedoTexture = tex;
        }
    }

    void Reload(const char* imageDir) {
        if (isLoaded) {
            UnloadTexture(albedoTexture);
        }
        SetTexture(imageDir);
    }

};