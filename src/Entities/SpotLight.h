#pragma once
#include "raylib.h"
#include <string>
#include <vector>

enum class LightKind {
    Directional,
    Point
};

struct Light {
    LightKind kind;
    bool active;
    Vector3 position;
    Vector3 target;
    Color color;

    unsigned int UiD;
    std::string name;

    int locActive;
    int locKind;
    int locPosition;
    int locTarget;
    int locColor;
};

class LightManager {
public:
    LightManager() = default;

    Light& AddLight(LightKind kind, Vector3 pos, Vector3 tgt, Color col, Shader shader) {
        lights.emplace_back();
        Light& light = lights.back();

        light.kind = kind;
        light.active = true;
        light.position = pos;
        light.target = tgt;
        light.color = col;

        light.UiD = UiDGenerator::GenerateUiD();
        light.name = "Light" + std::to_string(lights.size());

        int idx = static_cast<int>(lights.size() - 1);
        light.locActive   = GetShaderLocation(shader, TextFormat("lights[%i].active", idx));
        light.locKind     = GetShaderLocation(shader, TextFormat("lights[%i].kind", idx));
        light.locPosition = GetShaderLocation(shader, TextFormat("lights[%i].position", idx));
        light.locTarget   = GetShaderLocation(shader, TextFormat("lights[%i].target", idx));
        light.locColor    = GetShaderLocation(shader, TextFormat("lights[%i].color", idx));

        UpdateShader(shader, light);

        return light;
    }

    void UpdateAll(Shader shader) {
        for (auto& light : lights) {
            UpdateShader(shader, light);
        }
    }
    
    size_t Size() const { 
        return lights.size(); 
    }

private:
    std::vector<Light> lights;

    void UpdateShader(Shader shader, Light& light) {
        SetShaderValue(shader, light.locActive, &light.active, SHADER_UNIFORM_INT);

        int kindVal = static_cast<int>(light.kind);
        SetShaderValue(shader, light.locKind, &kindVal, SHADER_UNIFORM_INT);

        float pos[3] = { light.position.x, light.position.y, light.position.z };
        SetShaderValue(shader, light.locPosition, pos, SHADER_UNIFORM_VEC3);

        float tgt[3] = { light.target.x, light.target.y, light.target.z };
        SetShaderValue(shader, light.locTarget, tgt, SHADER_UNIFORM_VEC3);

        float col[4] = { light.color.r / 255.f, light.color.g / 255.f,
                         light.color.b / 255.f, light.color.a / 255.f };
        SetShaderValue(shader, light.locColor, col, SHADER_UNIFORM_VEC4);
    }
};
