#pragma once

#include "raylib.h"
#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"

#include "Camera/Camera.h"
#include "Entities/Rectangle.h"

class Renderer {
public:
    void Init();
    void RenderFrame(Camera2D currentCamera, const std::vector<RectangleObject>& rects);
    void ImGuiRender(bool CanEdit);
private:
    EditorCamera cam;
    RectangleObj rect;
};
