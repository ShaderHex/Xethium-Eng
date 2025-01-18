#include "raylib.h"
#include "rlImGui.h"
#include "iostream"
#include "Application.h"

bool Application::Init() {
    SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 600, "Xethium");

    rlImGuiSetup(true);

    return true;
}

void Application::Render() {
    BeginDrawing();
    Renderer Ren;
    Ren.Loop();
    EndDrawing();
}