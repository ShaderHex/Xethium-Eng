#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "raylib.h"


#include "Core/UiDgenerator.h"

struct RectangleObject {
    Vector2 position;
    Vector2 size;
    Color color;
    std::string name;
    unsigned int UiD;

    RectangleObject() = default;

    RectangleObject(float x, float y, float width, float height, Color color)
        : UiD(UiDGenerator::GenerateUiD()), position{ x, y }, size{ width, height }, color(color) {}
};


extern std::vector<RectangleObject> rectangles;

class RectangleObj {
public:
    void CreateRect(std::vector<RectangleObject>& rects);
};