#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "raylib.h"
#include "Core/UiDgenerator.h"

struct RectangleObject {
    Vector3 position;
    Vector3 size;
    Color color;
    std::string name;
    unsigned int UiD;

    RectangleObject() = default;

    RectangleObject(float x, float y, float z, float w, float h, float d, Color color)
        : UiD(UiDGenerator::GenerateUiD()),
          position{ x, y, z },
          size{ w, h, d },
          color(color) {}
};

extern std::vector<RectangleObject> rectangles;

class RectangleObj {
public:
    void CreateRect(std::vector<RectangleObject>& rects);
};
