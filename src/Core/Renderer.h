// Renderer.h
#pragma once
#include "raylib.h"

class Application;

class Renderer {
private:
    Application* app;
public:
    void Loop();
    void imguiInit();
    void imguiLoop(RenderTexture2D target);
};