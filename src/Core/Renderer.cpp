#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include "Renderer.h"
#include "Application.h"
#include "iostream"

static bool initialized = false;

void Renderer::Loop() {
    ClearBackground(RAYWHITE);
    DrawCircle(400, 300, 100, RED);
}

void Renderer::imguiInit() {
    rlImGuiBegin(); 

    int currentWindowWidth = GetScreenWidth();
    int currentWindowHeight = GetScreenHeight();

    static int lastWindowWidth = currentWindowWidth;
    static int lastWindowHeight = currentWindowHeight;
    app->SetupDocking(currentWindowWidth, currentWindowHeight);
    
    rlImGuiEnd();

}

void Renderer::imguiLoop(RenderTexture2D target) {
    rlImGuiBegin(); 
    int currentWindowWidth = GetScreenWidth();
    int currentWindowHeight = GetScreenHeight();

    static int lastWindowWidth = currentWindowWidth;
    static int lastWindowHeight = currentWindowHeight;
    
    if (currentWindowWidth != lastWindowWidth || currentWindowHeight != lastWindowHeight) {
        app->SetupDocking(currentWindowWidth, currentWindowHeight);
        lastWindowWidth = currentWindowWidth; 
        lastWindowHeight = currentWindowHeight;
    }

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();

    if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        rlImGuiImage(&target.texture); 
    }
    ImGui::End();

    if (ImGui::Begin("Components")) {
        ImGui::Text("Entity");
    }
    ImGui::End();
    
    rlImGuiEnd();
}
