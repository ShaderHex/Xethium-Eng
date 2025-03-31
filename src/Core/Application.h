#pragma once
#include "Renderer.h"
#include "raylib.h"
#include "rlImGui.h"
#include "Camera/Camera.h"

class Application {
private:
    Renderer renderer;
    CameraWrapper* camera;
    RenderTexture2D target;
    Vector2 lastViewportSize = { 1200, 600 };
public:
    bool Init();
    void Render();
    void SetupDocking(int currentWindowWidth, int currentWindowHeight);
    void CleanUp();
};