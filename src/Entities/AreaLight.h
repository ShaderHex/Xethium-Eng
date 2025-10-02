#pragma once

#include <string>
#include "raylib.h"
#include "raymath.h"
#include "Core/UiDgenerator.h"

#define XE_LIGHT_DIRECTIONAL 0
#define XE_LIGHT_POINT 1

struct LightEntity {
    int type;
    Vector3 position;
    Vector3 target;
    Color color;
    float intensity;
    std::string name;
    unsigned int UiD;
    bool enabled;
    RenderTexture2D* shadowMap;
    bool castsShadows = true;

    Matrix lightView;
    Matrix lightProj;
    Matrix lightViewProj;

    LightEntity()
        : type(XE_LIGHT_POINT),
          position{0.0f, 2.0f, 0.0f},
          target{0.0f, 0.0f, 0.0f},
          color(WHITE),
          intensity(1.0f),
          name("Light"),
          UiD(UiDGenerator::GenerateUiD()),
          enabled(true),
          shadowMap(nullptr),
          lightView(MatrixIdentity()),
          lightProj(MatrixIdentity()),
          lightViewProj(MatrixIdentity())
    {}

    LightEntity(int lightType, Vector3 pos, Color col, float intens, std::string lightName)
        : type(lightType),
          position(pos),
          target{0.0f, 0.0f, 0.0f},
          color(col),
          intensity(intens),
          name(lightName),
          UiD(UiDGenerator::GenerateUiD()),
          enabled(true),
          shadowMap(nullptr),
          lightView(MatrixIdentity()),
          lightProj(MatrixIdentity()),
          lightViewProj(MatrixIdentity())
    {}
};


class LightSystem {
public:
    std::vector<LightEntity> lights;
    Shader* shader = nullptr;

    LightSystem() {}

    void SetShader(Shader* s) {
        shader = s;
    }

    void AddLight(LightEntity light) {
        lights.push_back(light);
    }

    void CreateDefaultLight(RenderTexture2D& ShadowMap) {
        LightEntity light;
        light.name = "Light" + std::to_string(lights.size());
        light.shadowMap = &ShadowMap;
        lights.push_back(light);
    }

    void UpdateLights(Camera3D& camera) {
        if (!shader) return;

        SetShaderValue(*shader, GetShaderLocation(*shader, "viewPos"), &camera.position, SHADER_UNIFORM_VEC3);

        float ambient[4] = {0.2f, 0.2f, 0.2f, 1.0f};
        SetShaderValue(*shader, GetShaderLocation(*shader, "ambient"), ambient, SHADER_UNIFORM_VEC4);

        int lightCount = (lights.size() < 4) ? lights.size() : 4;
        
        for (int i = 0; i < lightCount; i++) {
            if (!lights[i].enabled) continue;
            
            std::string lightIndex = "lights[" + std::to_string(i) + "]";
            
            SetShaderValue(*shader, GetShaderLocation(*shader, (lightIndex + ".type").c_str()), 
                          &lights[i].type, SHADER_UNIFORM_INT);
            
            SetShaderValue(*shader, GetShaderLocation(*shader, (lightIndex + ".position").c_str()), 
                          &lights[i].position, SHADER_UNIFORM_VEC3);
            
            SetShaderValue(*shader, GetShaderLocation(*shader, (lightIndex + ".target").c_str()), 
                          &lights[i].target, SHADER_UNIFORM_VEC3);
            
            Vector4 lightColor = {
                (lights[i].color.r / 255.0f) * lights[i].intensity,
                (lights[i].color.g / 255.0f) * lights[i].intensity,
                (lights[i].color.b / 255.0f) * lights[i].intensity,
                1.0f
            };
            SetShaderValue(*shader, GetShaderLocation(*shader, (lightIndex + ".color").c_str()), 
                          &lightColor, SHADER_UNIFORM_VEC4);
        }
        
        for (int i = lightCount; i < 4; i++) {
            std::string lightIndex = "lights[" + std::to_string(i) + "]";
            int disabledType = -1;
            SetShaderValue(*shader, GetShaderLocation(*shader, (lightIndex + ".type").c_str()), 
                          &disabledType, SHADER_UNIFORM_INT);
        }
    }

    void UpdateLightMatrix(LightEntity& light, int shadowWidth, int shadowHeight) {
        if (light.type == XE_LIGHT_DIRECTIONAL) {
            light.lightView = MatrixLookAt(light.position, light.target, {0.0f, 1.0f, 0.0f});
            float orthoSize = 10.0f;
            light.lightProj = MatrixOrtho(-orthoSize, orthoSize, -orthoSize, orthoSize, 0.1f, 100.0f);
        } else {
            light.lightView = MatrixLookAt(light.position, light.target, {0.0f, 1.0f, 0.0f});
            light.lightProj = MatrixPerspective(90.0f * DEG2RAD, (float)shadowWidth / shadowHeight, 0.1f, 100.0f);
        }
        light.lightViewProj = MatrixMultiply(light.lightView, light.lightProj);
    }

    void DebugDraw() {
        for (auto& light : lights) {
            if (light.enabled) {
                DrawSphereWires(light.position, 0.2f, 8, 8, light.color);
            } else {
                DrawSphereWires(light.position, 0.2f, 8, 8, ColorAlpha(light.color, 0.3f));
            }
        }
    }
    
    LightEntity* GetLightByUiD(unsigned int UiD) {
        for (auto& light : lights) {
            if (light.UiD == UiD) {
                return &light;
            }
        }
        return nullptr;
    }
    Matrix GetLightMatrix(const LightEntity& light, int shadowWidth, int shadowHeight) {
        Matrix view, proj;
        
        if (light.type == 0) {
            view = MatrixLookAt(light.position, light.target, {0.0f, 1.0f, 0.0f});
            float orthoSize = 10.0f;
            proj = MatrixOrtho(-orthoSize, orthoSize, -orthoSize, orthoSize, 0.1f, 100.0f);
        } else {
            view = MatrixLookAt(light.position, light.target, {0.0f, 1.0f, 0.0f});
            proj = MatrixPerspective(90.0f * DEG2RAD, (float)shadowWidth / shadowHeight, 0.1f, 100.0f);
        }
        
        return MatrixMultiply(view, proj);
    }
};