#pragma once

#include <iostream>
#include <vector>

#include "raylib.h"

extern Camera2D camera;
extern Camera2D playCamera;

class EditorCamera {
public:
    Camera2D LoadCam(const std::string& filename);
    void InitCam();
    void UpdateCamera();
};