#pragma once
#include "defines.h"
#include "../headers/gameObject/color.h"

namespace XENGINE {
    // Platform
    extern "C" XAPI void Init(const char* title, int windowX, int windowY);
    extern "C" XAPI bool WindowShouldClose();
    extern "C" XAPI void StartDrawing(void* Shader);
    extern "C" XAPI void EndDrawing();
    extern "C" XAPI void CloseWindow();
    
    // Shader
    XAPI void* CreateShader(const char* vertexPath, const char* fragmentPath);
    XAPI void useShader(void* shader);

    // Game Object
    XAPI void CreateCube(float x, float y, float z, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ, Color color);
}
