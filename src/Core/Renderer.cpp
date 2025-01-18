#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include "Renderer.h"
#include "iostream"

void Renderer::Loop() {
    ClearBackground(RAYWHITE);
    DrawCircle(400, 300, 100, RED);
}

void Renderer::imguiLoop(RenderTexture2D target) {
    rlImGuiBegin(); 
#ifdef IMGUI_HAS_DOCK
		ImGui::DockSpaceOverViewport(0,  NULL, ImGuiDockNodeFlags_PassthruCentralNode);
#endif

        if (ImGui::Begin("Raylib Drawing inside ImGui", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            rlImGuiImage(&target.texture);
        }
        ImGui::End();

        if (ImGui::Begin("Component")) {
            ImGui::Text("Entity");
        }
        ImGui::End();

    rlImGuiEnd();
}