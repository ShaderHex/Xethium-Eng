#pragma once
#include "raylib.h"
#include <string>
#include <memory>
#include "Core/UiDgenerator.h"

class Entity {
public:
    unsigned int UiD;
    std::string name;
    Vector2 position;
    Color color;

    Entity(Vector2 pos, Color col);
    virtual ~Entity() = default;
    
    virtual void Draw() const = 0;
    virtual void DrawInspectorUI() = 0;
    virtual Rectangle GetBoundingBox() const = 0;
    virtual const char* GetTypeName() const = 0;
};