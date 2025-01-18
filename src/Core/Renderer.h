#pragma once
#include "raylib.h"

class Renderer {
public:
    void Loop();
    void imguiLoop(RenderTexture2D target);
};