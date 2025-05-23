#include <fstream>

#include <nlohmann/json.hpp>

#include "Camera.h"

Camera2D camera = { 0 };
Camera2D playCamera = {0};
static Vector2 panOrigin = {0};
static bool isPanning = false;

Camera2D EditorCamera::LoadCam(const std::string& filePath) {
    std::ifstream file(filePath);
    nlohmann::json j;
    file >> j;

    Camera2D cam = {0};

    for (auto& item : j) {
        if (item["name"] == "camera") {
            cam.target = { item["target"][0], item["target"][1] };
            cam.offset = { item["offset"][0], item["offset"][1] };
            cam.rotation = item["rotation"];
            cam.zoom = item["zoom"];
            break;
        }
    }

    return cam;
}

void EditorCamera::InitCam() {
    camera.target = (Vector2){ 0.0, 0.0 };
    camera.offset = (Vector2){ 0.0, 0.0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void EditorCamera::UpdateCamera() {
    if (IsKeyDown(KEY_D)) {
        camera.target.x += 900 * GetFrameTime();
    } 
    if (IsKeyDown(KEY_A)) {
        camera.target.x -= 900 * GetFrameTime();
    }
    if (IsKeyDown(KEY_W)) {
        camera.target.y -= 900 * GetFrameTime();
    }
    if (IsKeyDown(KEY_S)) {
        camera.target.y += 900 * GetFrameTime();
    }

    float zoomSpeed = 0.1f;
    camera.zoom += GetMouseWheelMove() * zoomSpeed;


    if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) {
        panOrigin = GetMousePosition();
        isPanning = true;
    } else if (IsMouseButtonReleased(MOUSE_MIDDLE_BUTTON)) {
        isPanning = false;
    }

    if (isPanning) {
        Vector2 mouseDelta = Vector2Subtract(panOrigin, GetMousePosition());
        mouseDelta = Vector2Scale(mouseDelta, 1.0f / camera.zoom); // Scale by zoom
        camera.target = Vector2Add(camera.target, mouseDelta);
        panOrigin = GetMousePosition(); // Update origin for smooth dragging
    }
}