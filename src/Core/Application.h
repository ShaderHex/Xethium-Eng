#pragma once
#include "Renderer.h"
#include "raylib.h"
#include "rlImGui.h"

class Application {
private:
    Renderer renderer;
public:
    bool Init();
    void Render();
    void SetupDocking();
    void CleanUp();
};