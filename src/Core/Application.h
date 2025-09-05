#pragma once

#include <vector>
#include <iostream>

#include "raylib.h"
#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"

#include "Renderer/Renderer.h"
#include "Entities/Rectangle.h"
#include "Camera/Camera.h"
#include "SceneManager.h"
#include "EngineState.h"

class Application {
public:
    void Init();
    void Run();
    bool CurrentGameMode();
    void Shutdown();

    static Mode currentMode;
private:
    Renderer renderer;
    EditorCamera cam;
    std::vector<RectangleObject> rectangles;
    std::vector<RectangleObject> circles;
};