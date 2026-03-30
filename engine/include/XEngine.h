#pragma once
#include "defines.h"
#include "../headers/gameObject/color.h"
#include "../headers/camera/camera.h"
#include "../headers/shader/shader.h"
#include "../headers/platform/platform.h"
#include "../headers/input/input.h"
#include "../headers/gameObject/gameObject.h"


namespace XENGINE {
    // Platform
    XAPI void Init(const char* title, int windowX, int windowY);
    XAPI bool WindowShouldClose();
    XAPI void StartDrawing(Shader::Shader* Shader, Camera::Camera Camera);
    XAPI void EndDrawing();
    XAPI void CloseWindow();
    XAPI float GetDeltaTime();
    
    // Shader
    XAPI Shader::Shader* CreateShader(const char* vertexPath, const char* fragmentPath);
    XAPI void useShader(void* shader);

    // Game Object
    XAPI GameObject::GameObject CreateCube(float x, float y, float z, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ, Color color, const char* texturePath = nullptr);

    // Camera
    XAPI Camera::Camera CreateCamera();

    // Input
    XAPI bool IsActionPressed(std::string action);
    XAPI bool IsActionReleased(std::string action);
    XAPI bool IsActionHeld(std::string action);
    XAPI void CreateAction(std::string action, int key);
}
