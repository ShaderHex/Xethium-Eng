#pragma once
#include <vector>
#include <string>

#include "raylib.h"

struct SphereObject {
    std::string name;
    long long UiD;
    Vector3 position;
    float radius;
    Color color;    
    bool enableBloom = false;
};