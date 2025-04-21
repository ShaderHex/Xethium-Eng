#include "Renderer.h"
#include "raylib.h"

void Renderer::Init() {
    // Any renderer-specific initialization can go here
    // For example: loading shaders, setting up render targets, etc.
    // For now, nothing fancy needed.
}

void Renderer::RenderFrame(Camera2D currentCamera) {
    ClearBackground(GRAY);
    BeginMode2D(currentCamera);
        for (const RectangleObject& rect : rectangles) {
            DrawRectangleV(rect.position, rect.size, rect.color);
        }
    EndMode2D();

}
