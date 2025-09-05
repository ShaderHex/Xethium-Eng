#include "Renderer.h"
#include "Core/Application.h"
#include "Core/SceneManager.h"
#include "imgui.h"
#include "rlImGui.h"
#include "ImGuizmo.h"
#include <raygizmo.h>

#include <filesystem>
#include <iostream>
#include <cstring>

namespace fs = std::filesystem;

int hoveredUiD = -1;
int selectedUiD = -1;

std::string currentPathWrite;
std::string fileNameWrite;
std::unordered_set<long long> usedUiDs;

Vector3 playCamPos = EditorCamera::playCamera.position;

void Renderer::Init() {
    // Renderer-specific initialization
}

bool CheckCollisionRayBox(Ray ray, BoundingBox box) {
    float tmin = (box.min.x - ray.position.x) / ray.direction.x;
    float tmax = (box.max.x - ray.position.x) / ray.direction.x;
    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (box.min.y - ray.position.y) / ray.direction.y;
    float tymax = (box.max.y - ray.position.y) / ray.direction.y;
    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax)) return false;
    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    float tzmin = (box.min.z - ray.position.z) / ray.direction.z;
    float tzmax = (box.max.z - ray.position.z) / ray.direction.z;
    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax)) return false;
    return true;
}

void RenderFileManagerPanel(const std::string& projectDir, std::vector<RectangleObject>& rects, Camera3D& camera, Camera3D& playCamera) {
    try {
        if (!fs::exists(projectDir) || !fs::is_directory(projectDir)) {
            ImGui::Begin("File Explorer");
            ImGui::Text("Error: Project directory '%s' does not exist!", projectDir.c_str());
            ImGui::End();
            return;
        }

        if (ImGui::Begin("File Explorer")) {
            static std::string currentPath = projectDir;
            ImGui::Text("Current Path: %s", currentPath.c_str());

            for (const auto& entry : fs::directory_iterator(currentPath)) {
                const std::string fileName = entry.path().filename().string();

                if (entry.is_directory()) {
                    if (ImGui::Button(("Open Folder: " + fileName).c_str())) {
                        currentPath = entry.path().string();
                    }
                } else {
                    if (ImGui::Button(("Select File: " + fileName).c_str())) {
                        rects = SceneManager::LoadScene(entry.path().string(), camera, playCamera);
                        fileNameWrite = fileName;
                        currentPathWrite = currentPath;
                        std::cout << "Loaded: " << entry.path().string() << std::endl;
                    }
                }
            }

            ImGui::End();
        }
    } catch (const std::exception& e) {
        ImGui::Begin("File Explorer");
        ImGui::Text("Error: %s", e.what());
        ImGui::End();
    }
}

void Renderer::RenderFrame(Camera3D& currentCamera, std::vector<RectangleObject>& rects) {
    ClearBackground(GRAY);
    BeginMode3D(currentCamera);

    Ray ray = GetMouseRay(GetMousePosition(), currentCamera);
    hoveredUiD = -1;
    if (Application::currentMode == MODE_EDIT) {
        playCamPos.x += 0.5f;
        DrawCube(EditorCamera::playCamera.position, 1, 1, 1, BLUE);
        DrawCube(playCamPos, 1, 1, 1, BLUE);
    }
    for (auto& r : rects) {
        BoundingBox box = {
            Vector3Subtract(r.position, Vector3Scale(r.size, 0.5f)),
            Vector3Add(r.position, Vector3Scale(r.size, 0.5f))
        };

        DrawCube(r.position, r.size.x, r.size.y, r.size.z, r.color);

        if (Application::currentMode == MODE_EDIT) {
            if (CheckCollisionRayBox(ray, box)) {
                hoveredUiD = r.UiD;
                DrawCubeWires(r.position, r.size.x, r.size.y, r.size.z, YELLOW);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    selectedUiD = r.UiD;
                }
            }

            if (selectedUiD == r.UiD) {
                DrawCubeWires(r.position, r.size.x, r.size.y, r.size.z, WHITE);
            }
        }
    }

    for (auto& r : rects) {
        if (selectedUiD == r.UiD) {
            static Transform gizmoTransform = GizmoIdentity();
            gizmoTransform.translation = r.position;
            gizmoTransform.scale = r.size;

            DrawGizmo3D(GIZMO_TRANSLATE | GIZMO_ROTATE | GIZMO_SCALE, &gizmoTransform);

            r.position = gizmoTransform.translation;
            r.size = gizmoTransform.scale;
        }
    }

    EndMode3D();
}

static void GetCameraMatrices(Camera3D& cam, float view[16], float proj[16]) {
    Matrix viewMat = GetCameraMatrix(cam);
    Matrix projMat = MatrixPerspective(cam.fovy * DEG2RAD, (float)GetScreenWidth() / GetScreenHeight(), 0.01f, 1000.0f);
    memcpy(view, &viewMat, sizeof(float) * 16);
    memcpy(proj, &projMat, sizeof(float) * 16);
}

long long GenerateUniqueUiD() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long long> dis(1, 999999999999999);

    long long newUiD;

    do {
        newUiD = dis(gen);
    } while (usedUiDs.count(newUiD));

    usedUiDs.insert(newUiD);

    return newUiD;
}
void Renderer::ImGuiRender(bool CanEdit, std::vector<RectangleObject>& rects, Camera3D*& currentCamera, Camera3D* editorCam, Camera3D* playCam) {
    rlImGuiBegin();

    ImGui::Begin("Inspector");

    if (ImGui::Button("Create Cube") && CanEdit) {
        RectangleObject obj;
        obj.position = {0.0f, 1.0f, 0.0f};
        obj.size = {1.0f, 1.0f, 1.0f};
        obj.color = RED;
        obj.UiD = GenerateUniqueUiD();
        rects.push_back(obj);
    }

    if (ImGui::Button("Select Object")) ImGui::OpenPopup("Select UiD");

    if (ImGui::BeginPopup("Select UiD")) {
        for (const auto& rect : rects) {
            if (ImGui::Selectable(("UiD: " + std::to_string(rect.UiD)).c_str()))
                selectedUiD = rect.UiD;
        }
        ImGui::EndPopup();
    }

    for (auto& rect : rects) {
    if (rect.UiD == selectedUiD) {
            static GizmoManager gizmo;
            gizmo.transform.translation = rect.position;
            gizmo.transform.scale = rect.size;

            gizmo.Draw(GIZMO_TRANSLATE | GIZMO_ROTATE | GIZMO_SCALE);

            Matrix m = gizmo.GetMatrix();
            rect.position.x = m.m12;
            rect.position.y = m.m13;
            rect.position.z = m.m14;
            rect.size.x = gizmo.transform.scale.x;
            rect.size.y = gizmo.transform.scale.y;
            rect.size.z = gizmo.transform.scale.z;
        }
    }


    if (ImGui::Button("Save Scene") && !fileNameWrite.empty()) {
        SceneManager::SaveScene(rects, *editorCam, *playCam, currentPathWrite + "/" + fileNameWrite);
        std::cout << "Saved to: " << currentPathWrite + "/" + fileNameWrite << std::endl;
    }

    if (ImGui::Button("Create Scene")) ImGui::OpenPopup("Create Scene Popup");

    if (ImGui::BeginPopupModal("Create Scene Popup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char SceneName[128] = "";
        ImGui::InputText("Name", SceneName, IM_ARRAYSIZE(SceneName));

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            if (strlen(SceneName) > 0) SceneManager::CreateScene(SceneName);
            else std::cerr << "[UI] Error: Scene name cannot be empty!\n";

            ImGui::CloseCurrentPopup();
            SceneName[0] = '\0';
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }

    ImGui::End();

    ImGui::Begin("Play");
    if (Application::currentMode == MODE_EDIT) {
        if (ImGui::Button("Play")) {
            Application::currentMode = MODE_PLAY;
            currentCamera = playCam;
        }
    } else {
        if (ImGui::Button("Stop Playing")) {
            Application::currentMode = MODE_EDIT;
            currentCamera = editorCam;
        }
    }
    ImGui::End();

    RenderFileManagerPanel("project/", rects, *editorCam, *playCam);

    rlImGuiEnd();
}