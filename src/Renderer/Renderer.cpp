#include "Renderer.h"
#include "Core/Application.h"
void Renderer::Init() {
    // Any renderer-specific initialization can go here
    // For example: loading shaders, setting up render targets, etc.
    // For now, nothing fancy needed.
}

void Renderer::RenderFrame(Camera2D currentCamera, const std::vector<RectangleObject>& rects) {
    ClearBackground(GRAY);
    BeginMode2D(currentCamera);
        for (const auto& r : rects) {
            DrawRectangleV(r.position, r.size, r.color);
        }
    EndMode2D();

}

void Renderer::ImGuiRender(bool CanEdit, std::vector<RectangleObject>& rects) {
    rlImGuiBegin();
        ImGui::Begin("Inspector");
            if (ImGui::Button("Create")) {
                    if (CanEdit) {
                        rectang.CreateRect(rects);
                    }
            }
            if (ImGui::Button("Create Camera")) {

            }
        ImGui::End();
    rlImGuiEnd();
}