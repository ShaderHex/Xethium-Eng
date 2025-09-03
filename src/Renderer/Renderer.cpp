#include "Renderer.h"
#include "Core/Application.h"
#include "Core/SceneManager.h"
#include "imgui.h"
#include "rlImGui.h"
#include "ImGuizmo.h"

#include <filesystem>
#include <iostream>
#include <cstring>

namespace fs = std::filesystem;

int hoveredUiD = -1;
int selectedUiD = -1;

std::string currentPathWrite;
std::string fileNameWrite;

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

    EndMode3D();
}

static void GetCameraMatrices(Camera3D& cam, float view[16], float proj[16]) {
    Matrix viewMat = GetCameraMatrix(cam);
    Matrix projMat = MatrixPerspective(cam.fovy * DEG2RAD, (float)GetScreenWidth() / GetScreenHeight(), 0.01f, 1000.0f);
    memcpy(view, &viewMat, sizeof(float) * 16);
    memcpy(proj, &projMat, sizeof(float) * 16);
}

void Renderer::ImGuiRender(bool CanEdit, std::vector<RectangleObject>& rects, Camera3D*& currentCamera, Camera3D* editorCam, Camera3D* playCam) {
    rlImGuiBegin();

    ImGui::Begin("Inspector");

    if (ImGui::Button("Create Cube") && CanEdit) {
        RectangleObject obj;
        obj.position = {0.0f, 1.0f, 0.0f};
        obj.size = {1.0f, 1.0f, 1.0f};
        obj.color = RED;
        obj.UiD = rand() % 10000;
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
            ImGui::Text("Modify Object: %d", rect.UiD);
            ImGui::DragFloat3("Position", &rect.position.x);
            ImGui::DragFloat3("Size", &rect.size.x);

            float tempColor[3] = {rect.color.r / 255.0f, rect.color.g / 255.0f, rect.color.b / 255.0f};
            if (ImGui::ColorEdit3("Color", tempColor)) {
                rect.color.r = static_cast<unsigned char>(tempColor[0] * 255);
                rect.color.g = static_cast<unsigned char>(tempColor[1] * 255);
                rect.color.b = static_cast<unsigned char>(tempColor[2] * 255);
            }

            // ImGuizmo translation
            if (CanEdit) {
                float view[16], proj[16], objectMatrix[16];
                GetCameraMatrices(*editorCam, view, proj);

                Matrix m = MatrixTranslate(rect.position.x, rect.position.y, rect.position.z);
                memcpy(objectMatrix, &m, sizeof(float) * 16);

                ImGuizmo::SetRect(0, 0, (float)GetScreenWidth(), (float)GetScreenHeight());
                ImGuizmo::SetDrawlist();
                ImGuizmo::Manipulate(view, proj, ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, objectMatrix);

                if (ImGuizmo::IsUsing()) {
                    rect.position.x = objectMatrix[12];
                    rect.position.y = objectMatrix[13];
                    rect.position.z = objectMatrix[14];
                }
            }
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

    // Play/Edit toggle
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
