#pragma once
#include "Entity.h"
#include "RectangleObject.h"
#include "CircleObject.h"

class EntityFactory {
public:
    static std::unique_ptr<Entity> CreateRectangle(Vector2 pos = {0, 0}, 
                                                 Vector2 size = {100, 100}, 
                                                 Color col = RED) {
        return std::make_unique<RectangleObject>(pos, size, col);
    }

    static std::unique_ptr<Entity> CreateCircle(Vector2 pos = {0, 0},
                                              float radius = 50.0f,
                                              Color col = BLUE) {
        return std::make_unique<CircleObject>(pos, radius, col);
    }
};