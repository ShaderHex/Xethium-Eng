#include "Renderer.h"
#include "raylib.h"

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
