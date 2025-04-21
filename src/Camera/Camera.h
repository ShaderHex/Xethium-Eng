#pragma once

#include <iostream>
#include <vector>

#include "raylib.h"

extern Camera2D camera;
extern Camera2D playCamera;

class EditorCamera {
public:
    void InitCam();
    void UpdateCamera();
};