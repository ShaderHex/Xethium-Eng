#pragma once
#include "raylib.h"
#include "Camera/Camera.h"

class Application;

class Renderer {
private:
    Application* app;
    CameraWrapper* camera;
    Vector2 viewportSize = { 1200, 600 };
public:
    Renderer(CameraWrapper* cam = nullptr) : camera(cam) {}
    void SetCamera(CameraWrapper* cam) { camera = cam; }
    void Init();
    void Loop();
    void imguiInit();
    void imguiLoop(RenderTexture2D target);
    Vector2 GetViewportSize() const { return viewportSize; }
};