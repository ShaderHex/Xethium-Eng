#include "Camera.h"

Camera2D camera = { 0 };
Camera2D playCamera = {0};

void EditorCamera::InitCam() {
    camera.target = (Vector2){ 0.0, 0.0 };
    camera.offset = (Vector2){ 0.0, 0.0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void EditorCamera::UpdateCamera() {
    if (IsKeyDown(KEY_D)) {
        camera.target.x += 0.5;
    } 
    if (IsKeyDown(KEY_A)) {
        camera.target.x -= 0.5;
    }
    if (IsKeyDown(KEY_W)) {
        camera.target.y -= 0.5;
    }
    if (IsKeyDown(KEY_S)) {
        camera.target.y += 0.5;
    }
}