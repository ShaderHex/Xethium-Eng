#pragma once

#include "raylib.h"

#include <vector>

struct RectangleObject {
    Vector2 position;
    Vector2 size;
    Color color;
};

extern std::vector<RectangleObject> rectangles;