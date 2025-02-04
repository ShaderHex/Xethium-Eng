#pragma once
#include "Renderer.h"
#include "raylib.h"
#include "rlImGui.h"
#include "Camera/Camera.h"

class Application {
private:
    Renderer renderer;
    CameraWrapper* camera;
public:
    bool Init();
    void Render();
    void SetupDocking(int currentWindowWidth, int currentWindowHeight);
    void CleanUp();
};