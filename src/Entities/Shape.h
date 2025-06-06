#pragma once
#include <string>
#include "raylib.h"

class Shape {
public:
    int UiD;
    std::string name;
    Vector2 position;
    Color color;
    virtual void Draw() const = 0;
    virtual ~Shape() {}
};