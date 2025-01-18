#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"
#include "iostream"
#include "Application.h"

int screenWidth = 1200, screenHeight = 600;
RenderTexture2D target;

bool Application::Init() {
    SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Xethium");

    rlImGuiSetup(true);
    target = LoadRenderTexture(screenWidth, screenHeight);
    
    std::cout<<"Init docking\n";
#ifdef IMGUI_HAS_DOCK
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif

    return true;
}

void Application::Render() {

    BeginDrawing();
    ClearBackground(BLACK);

    BeginTextureMode(target);

    renderer.Loop();

    EndTextureMode();

    renderer.imguiLoop(target);
    EndDrawing();
}

void Application::CleanUp() {
    rlImGuiShutdown();
    CloseWindow();
}