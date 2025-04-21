#pragma once

#include <vector>
#include <iostream>

#include "raylib.h"

#include "Renderer/Renderer.h"
#include "Entities/Rectangle.h"
#include "Camera/Camera.h"
#include "SceneManager.h"

class Application {
public:
    void Init();
    void Run();
    void Shutdown();
private:
    Renderer renderer;
    EditorCamera cam;
    std::vector<RectangleObject> rectangles;
};