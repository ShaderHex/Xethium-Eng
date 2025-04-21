#pragma once

#include "Camera/Camera.h"
#include "Entities/Rectangle.h"

class Renderer {
public:
    void Init();
    void RenderFrame(Camera2D currentCamera, const std::vector<RectangleObject>& rects);
private:
    EditorCamera cam;
};
