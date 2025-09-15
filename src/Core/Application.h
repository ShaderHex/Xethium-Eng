#pragma once

#include <vector>
#include <iostream>

#include "raylib.h"
#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"
#include "sol/sol.hpp"

#include "Renderer/Renderer.h"
#include "Entities/Rectangle.h"
#include "Camera/Camera.h"
#include "SceneManager.h"
#include "EngineState.h"
#include "Core/Scripting.h"
#include "Entities/Sphere.h"

class Application {
public:
    void Init();
    void Run();
    bool CurrentGameMode();
    void Shutdown();

    static Mode currentMode;
    Scripting scripting;
private:
    Renderer renderer;
    EditorCamera cam;
    std::vector<RectangleObject> rectangles;
    std::vector<SphereObject> sphere;
};