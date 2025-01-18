#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include "Renderer.h"
#include "iostream"

void Renderer::Loop() {
    ClearBackground(RAYWHITE);

        DrawRectangle(100, 100, 100, 100, RED);

        rlImGuiBegin();
        if (ImGui::Begin("Window")) {
            ImGui::Text("A window");
        }
        ImGui::End();
        rlImGuiEnd();

}