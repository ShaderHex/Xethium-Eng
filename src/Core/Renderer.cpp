#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include "Renderer.h"
#include "Application.h"
#include "iostream"
#include "imgui_impl_raylib.h"
#include <cstdint>

Vector3 CubePos;

void Renderer::Init() {
    CubePos = {0, 0, 0};
}

void Renderer::Loop() {
    if (!camera) {
        std::cerr << "Camera is not set!" << std::endl;
        return;
    }

    BeginMode3D(camera->camera);
    ClearBackground(DARKGRAY);
    DrawCube(CubePos, 2.0f, 2.0f, 2.0f, RED);
    EndMode3D();
}

void Renderer::imguiInit() {
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromFileTTF("fonts/roboto.ttf", 18.0f);

    if (!font) {
        std::cout << "Couldn't load the font!" << std::endl;
    } else {
        std::cout << "Font loaded successfully." << std::endl;
    }
    IM_ASSERT(font != nullptr);
    ImGui_ImplRaylib_BuildFontAtlas();

    io.FontDefault = font;
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

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Once);
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

    ImGui::DockSpace(dockspace_id, ImVec2(0, 0), 
    ImGuiDockNodeFlags_PassthruCentralNode | 
    ImGuiDockNodeFlags_NoUndocking
);
    ImGui::End();

    if (ImGui::Begin("Viewport", nullptr, 
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) 
    {
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        
        viewportSize.x = std::max(viewportSize.x, 1.0f);
        viewportSize.y = std::max(viewportSize.y, 1.0f);

        if (target.texture.id > 0) {
            rlImGuiImageRect(
                &target.texture, 
                viewportSize.x, 
                viewportSize.y,
                Rectangle{0, 0, (float)target.texture.width, -(float)target.texture.height}
            );
        }
    }
    ImGui::End();
    if (ImGui::Begin("Components", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
    ImGui::Text("Components");
}
    ImGui::End();
    if (ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        ImGui::Text("Inspector");
        if (ImGui::CollapsingHeader("Position")) {
            ImGui::DragFloat3("Object Position", &CubePos.x, 0.1f);
        }
    }
    ImGui::End();
    if (ImGui::Begin("File Manager",nullptr,  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        ImGui::Text("Entity");
    }
    ImGui::End();

    rlImGuiEnd();
}