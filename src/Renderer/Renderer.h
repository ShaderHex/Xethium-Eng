#pragma once

#include <fstream>

#include "raylib.h"
#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"

#include "Camera/Camera.h"
#include "Entities/Rectangle.h"
#include "Core/FileManager.h"

class Renderer {
public:
    void Init();
    void RenderFrame(Camera2D currentCamera, std::vector<RectangleObject>& rects);
    void ImGuiRender(bool CanEdit, std::vector<RectangleObject>& rects, Camera2D currentCamera);
    void HandleInput(std::vector<RectangleObject>& rects, Camera2D camera);

    int hoveredUiD = -1;
    int selectedUiD = -1;
    bool isDragging = false;
    Vector2 dragOffset = {0, 0};
    

private:
    EditorCamera cam;
    RectangleObj rectang;
    FileManager filemanager;
};