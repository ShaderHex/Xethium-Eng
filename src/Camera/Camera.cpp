#include "Camera.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Camera3D EditorCamera::editorCamera = {0};
Camera3D EditorCamera::playCamera   = {0};

static bool isPanning = false;
static Vector2 panOrigin = {0};

Camera3D EditorCamera::LoadCam(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) return {0};

    json j;
    file >> j;
    file.close();

    Camera3D cam = {0};

    for (auto& item : j) {
        if (item["name"] == "camera") {
            cam.position = { item["position"][0], item["position"][1], item["position"][2]};
            cam.target   = { item["target"][0],   item["target"][1],   item["target"][2] };
            cam.up       = { item["up"][0],       item["up"][1],       item["up"][2]};
            cam.fovy     = item["fovy"];
            cam.projection = CAMERA_PERSPECTIVE;
            break;
        }
    }

    return cam;
}

void EditorCamera::InitCam() {
    editorCamera.position   = { 0.0f, 5.0f, 10.0f };
    editorCamera.target     = { 0.0f, 5.0f, 9.0f };
    editorCamera.up         = { 0.0f, 1.0f, 0.0f };
    editorCamera.fovy       = 60.0f;
    editorCamera.projection = CAMERA_PERSPECTIVE;

    DisableCursor();
}

void EditorCamera::InitPlayCam() {
    playCamera.position   = { 0.0f, 5.0f, 10.0f };
    playCamera.target     = { 0.0f, 5.0f, 9.0f };
    playCamera.up         = { 0.0f, 1.0f, 0.0f };
    playCamera.fovy       = 45.0f;
    playCamera.projection = CAMERA_PERSPECTIVE;

    DisableCursor();
}

void EditorCamera::UpdateEditorCamera() {
    static float yaw = 0.0f;
    static float pitch = 0.0f;
    float speed = 10.0f * GetFrameTime();
    
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        HideCursor();
        Vector2 delta = GetMouseDelta();
        yaw   -= delta.x * 0.003f;
        pitch -= delta.y * 0.003f;

        if (pitch > 1.5f) pitch = 1.5f;
        if (pitch < -1.5f) pitch = -1.5f;
    }
    else {
        ShowCursor();
    }

    Vector3 forward;
    forward.x = cosf(pitch) * sinf(yaw);
    forward.y = sinf(pitch);
    forward.z = cosf(pitch) * cosf(yaw);
    forward = Vector3Normalize(forward);

    Vector3 worldUp = {0.0f, 1.0f, 0.0f};
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, worldUp));
    Vector3 up    = Vector3Normalize(Vector3CrossProduct(right, forward));

    if (IsKeyDown(KEY_W)) {
        editorCamera.position = Vector3Add(editorCamera.position, Vector3Scale(forward, speed));
    }
    if (IsKeyDown(KEY_S)) {
        editorCamera.position = Vector3Subtract(editorCamera.position, Vector3Scale(forward, speed));
    }
    if (IsKeyDown(KEY_A)) {
        editorCamera.position = Vector3Subtract(editorCamera.position, Vector3Scale(right, speed));
    }
    if (IsKeyDown(KEY_D)) {
        editorCamera.position = Vector3Add(editorCamera.position, Vector3Scale(right, speed));
    }

    editorCamera.target = Vector3Add(editorCamera.position, forward);
}


void EditorCamera::UpdatePlayCamera() {
    float speed = 10.0f * GetFrameTime();

    Vector3 forward = Vector3Normalize(Vector3Subtract(playCamera.target, playCamera.position));
    Vector3 right   = Vector3Normalize(Vector3CrossProduct(forward, playCamera.up));

    if (IsKeyDown(KEY_W)) {
        playCamera.position = Vector3Add(playCamera.position, Vector3Scale(forward, speed));
        playCamera.target   = Vector3Add(playCamera.target,   Vector3Scale(forward, speed));
    }
    if (IsKeyDown(KEY_S)) {
        playCamera.position = Vector3Subtract(playCamera.position, Vector3Scale(forward, speed));
        playCamera.target   = Vector3Subtract(playCamera.target,   Vector3Scale(forward, speed));
    }
    if (IsKeyDown(KEY_A)) {
        playCamera.position = Vector3Subtract(playCamera.position, Vector3Scale(right, speed));
        playCamera.target   = Vector3Subtract(playCamera.target,   Vector3Scale(right, speed));
    }
    if (IsKeyDown(KEY_D)) {
        playCamera.position = Vector3Add(playCamera.position, Vector3Scale(right, speed));
        playCamera.target   = Vector3Add(playCamera.target,   Vector3Scale(right, speed));
    }

    Vector2 delta = GetMouseDelta();
    float yaw   = -delta.x * 0.003f;
    float pitch = -delta.y * 0.003f;

    Matrix rot = MatrixRotateXYZ({ pitch, yaw, 0.0f });
    Vector3 forwardNew = Vector3Transform(forward, rot);

    playCamera.target = Vector3Add(playCamera.position, forwardNew);
}