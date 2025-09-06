#pragma once
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

struct Skybox {
    Model model;
    Texture2D texture;

    void Load(const char* filePath, float sphereRadius = 500.0f) {
        texture = LoadTexture(filePath);
        if (texture.id == 0) {
            return;
        }
        SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);

        Mesh sphereMesh = GenMeshSphere(sphereRadius, 32, 32);
        model = LoadModelFromMesh(sphereMesh);

        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    }

    void Unload() {
        UnloadTexture(texture);
        UnloadModel(model);
    }

    void Draw(Camera3D& cam) {
        if (model.meshCount == 0) return; 

        rlDisableBackfaceCulling();
        
        rlDisableDepthMask();

        rlPushMatrix();
            rlTranslatef(cam.position.x, cam.position.y, cam.position.z);
            
            DrawModel(model, {0, 0, 0}, 1.0f, WHITE);
        rlPopMatrix();
        
        rlEnableDepthMask();
        rlEnableBackfaceCulling();
    }
};
