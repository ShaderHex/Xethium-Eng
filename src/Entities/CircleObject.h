#pragma once
#include "Entity.h"

class CircleObject : public Entity {
public:
    float radius;

    CircleObject(Vector2 pos, float radius, Color col);
    
    void Draw() const override;
    void DrawInspectorUI() override;
    Rectangle GetBoundingBox() const override;
    const char* GetTypeName() const override { return "Circle"; }
};