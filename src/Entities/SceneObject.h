#pragma once

#include <vector>
#include "Rectangle.h"
#include "Entities/AreaLight.h"

struct SceneData {
    std::vector<RectangleObject> rectangles;
    std::vector<LightEntity> lights;
};