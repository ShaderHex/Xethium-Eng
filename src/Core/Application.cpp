#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"
#include "iostream"
#include "Application.h"
#include "imgui_internal.h"

int screenWidth = 1200, screenHeight = 600;
RenderTexture2D target;
void SetupDocking();

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
    renderer.imguiInit();
    
    std::cout<<"Init docking done\n";
    return true;
}

void Application::Render() {
    BeginDrawing();
    ClearBackground(BLACK);
    BeginTextureMode(target);
    renderer.Loop();
    EndTextureMode();

    renderer.imguiLoop(target);
    ImGui::Render();
    EndDrawing();
}

void Application::CleanUp() {
    rlImGuiShutdown();
    CloseWindow();
}

void Application::SetupDocking(int currentWindowWidth, int currentWindowHeight) {
    ImGuiID dockspace_id = ImGui::GetID("MyDockspace");

    ImGui::DockBuilderRemoveNode(dockspace_id);

    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None);
    ImGui::DockBuilderSetNodeSize(dockspace_id, ImVec2(currentWindowWidth, currentWindowHeight));

    ImGuiID dock_main = dockspace_id;
    ImGuiID dock_left = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Left, 0.25f, nullptr, &dock_main);
    ImGuiID dock_bottom = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Down, 0.25f, nullptr, &dock_main);
    ImGuiID dock_right = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Right, 0.2f, nullptr, &dock_main);

    ImGui::DockBuilderDockWindow("Components", dock_left);
    ImGui::DockBuilderDockWindow("Viewport", dock_right);

    ImGui::DockBuilderFinish(dockspace_id);
}