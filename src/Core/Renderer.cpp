#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include "Renderer.h"
#include "Application.h"
#include "iostream"
#include "imgui_impl_raylib.h"

void Renderer::Loop() {
    if (!camera) {
        std::cerr << "Camera is not set!" << std::endl;
        return;
    }

    BeginMode3D(camera->camera);
    ClearBackground(DARKGRAY);
    DrawCube({0, 0, 0}, 2.0f, 2.0f, 2.0f, RED);
    
    std::cout << "Position: (" 
              << camera->camera.position.x << ", " 
              << camera->camera.position.y << ", " 
              << camera->camera.position.z << ")" 
              << std::endl;    
    
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
    ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();

    if (ImGui::Begin("Viewport")) {
        rlImGuiImage(&target.texture); 
    }
    ImGui::End();

    if (ImGui::Begin("Components")) {
        ImGui::Text("Components");
    }
    ImGui::End();
    if (ImGui::Begin("Inspector")) {
        ImGui::Text("Inspector");
    }
    ImGui::End();
    if (ImGui::Begin("File Manager")) {
        ImGui::Text("Entity");
    }
    ImGui::End();

    rlImGuiEnd();
}