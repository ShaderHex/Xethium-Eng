#pragma once
#include "raylib.h"
#include "Camera/Camera.h"

class Application;

class Renderer {
private:
    Application* app;
    CameraWrapper* camera;
public:
    Renderer(CameraWrapper* cam = nullptr) : camera(cam) {}
    void SetCamera(CameraWrapper* cam) { camera = cam; }
    void Loop();
    void imguiInit();
    void imguiLoop(RenderTexture2D target);
};