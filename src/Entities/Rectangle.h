#pragma once

#include "raylib.h"

#include <vector>

struct RectangleObject {
    Vector2 position;
    Vector2 size;
    Color color;

    RectangleObject() = default;

    RectangleObject(float x, float y, float width, float height, Color color)
        : position{ x, y }, size{ width, height }, color(color) {}
};


extern std::vector<RectangleObject> rectangles;