#include "Renderer.h"
#include "Core/Application.h"
#include "Core/SceneManager.h"
#include "imgui.h"
#include "rlImGui.h"
#include "imgui_internal.h"
#include "ImGuizmo.h"
#include <raygizmo.h>

#include <filesystem>
#include <iostream>
#include <cstring>

namespace fs = std::filesystem;

int hoveredUiD = -1;
int selectedUiD = -1;
int CubeNumber;

std::string currentPathWrite;
std::string fileNameWrite;
std::unordered_set<long long> usedUiDs;

Vector3 playCamPos = EditorCamera::playCamera.position;

Skybox skybox;

RenderTexture2D target;

void DrawEngineGrid(int slices = 1000, float spacing = 1.0f) {
    int halfSlices = slices / 2;

    rlBegin(RL_LINES);
    for (int i = -halfSlices; i <= halfSlices; i++) {
        if (i == 0)
            rlColor3f(1.0f, 0.0f, 0.0f); 
        else if (i % 10 == 0)
            rlColor3f(1.0f, 1.0f, 1.0f); 
        else
            rlColor3f(0.6f, 0.6f, 0.6f); 

        rlVertex3f((float)i*spacing, 0.0f, (float)-halfSlices*spacing);
        rlVertex3f((float)i*spacing, 0.0f, (float)halfSlices*spacing);

        rlVertex3f((float)-halfSlices*spacing, 0.0f, (float)i*spacing);
        rlVertex3f((float)halfSlices*spacing, 0.0f, (float)i*spacing);
    }
    rlEnd();
}

void Renderer::Init() {

    IMGUI_CHECKVERSION();
ImGui::CreateContext();
ImGuiIO& io = ImGui::GetIO();
io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


    postShader = LoadShader(0, "project/shaders/postProcessing.fs");
    if (postShader.id == 0) {
        std::cout << "Shader failed to load!" << std::endl;
    } else {
        std::cout << "Shader loaded!\n";
    }

    skybox.Load("project/assets/skybox/default/skybox.png", false);
    target = LoadRenderTexture(1200, 800);
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
    if (IsWindowResized()) {
        UnloadRenderTexture(target);
        target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    }

    BeginTextureMode(target);
        ClearBackground(BLACK);
        
        BeginMode3D(currentCamera);
            rlDisableDepthTest();
            skybox.Draw(currentCamera);
            rlEnableDepthTest();
            
            DrawEngineGrid();

            Ray ray = GetMouseRay(GetMousePosition(), currentCamera);
            hoveredUiD = -1;
            if (Application::currentMode == MODE_EDIT) {
                playCamPos.x += 0.5f;
                DrawCube(EditorCamera::playCamera.position, 1, 1, 1, BLUE);
                DrawCube(playCamPos, 1, 1, 1, BLUE);
            }

            for (auto& r : rects) {
                DrawCube(r.position, r.size.x, r.size.y, r.size.z, r.color);

                if (Application::currentMode == MODE_EDIT) {
                    BoundingBox box = {
                        Vector3Subtract(r.position, Vector3Scale(r.size, 0.5f)),
                        Vector3Add(r.position, Vector3Scale(r.size, 0.5f))
                    };

                    if (CheckCollisionRayBox(ray, box)) {
                        hoveredUiD = r.UiD;
                        DrawCubeWires(r.position, r.size.x, r.size.y, r.size.z, YELLOW);
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) selectedUiD = r.UiD;
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
    EndTextureMode();

    ApplyPostProcessing();
}


void Renderer::ApplyPostProcessing() {
    BeginShaderMode(postShader);
        DrawTextureRec(
            target.texture,
            {0, 0, (float)target.texture.width, -(float)target.texture.height},
            {0, 0},
            WHITE
        );
    EndShaderMode();
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
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);


    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()), ImGuiCond_Always);

    ImGuiWindowFlags dockspaceFlags = ImGuiWindowFlags_NoTitleBar |
                                     ImGuiWindowFlags_NoResize |
                                     ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoCollapse |
                                     ImGuiWindowFlags_NoBringToFrontOnFocus |
                                     ImGuiWindowFlags_NoNavFocus |
                                     ImGuiWindowFlags_NoBackground;


    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0,0,0,0));

    ImGui::Begin("DockSpace", nullptr, dockspaceFlags);

    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor();

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0,0), ImGuiDockNodeFlags_PassthruCentralNode);


    static bool docked = false;
    if (!docked) {
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

        ImGuiID top_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.08f, nullptr, &dockspace_id);
        ImGuiID right_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, nullptr, &dockspace_id);
        ImGuiID bottom_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.30f, nullptr, &dockspace_id);

        ImGui::DockBuilderDockWindow("Play", top_id);
        ImGui::DockBuilderDockWindow("Inspector", right_id);
        ImGui::DockBuilderDockWindow("File Explorer", bottom_id);

        ImGui::DockBuilderFinish(dockspace_id);
        docked = true;
    }

    ImGui::End();


    ImGui::Begin("Inspector");

    if (ImGui::Button("Create Cube") && CanEdit) {
        
        RectangleObject obj;
        obj.position = {0.0f, 1.0f, 0.0f};
        obj.size = {1.0f, 1.0f, 1.0f};
        obj.color = RED;
        obj.UiD = GenerateUniqueUiD();
        obj.name = "Cube" + std::to_string(CubeNumber);
        CubeNumber++;
        rects.push_back(obj);
        std::cout << "Created cube with name: " << obj.name << std::endl;
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

                ImGui::Text("Modify Object: %d", rect.UiD);
            ImGui::DragFloat3("Position", &rect.position.x);
            ImGui::DragFloat3("Size", &rect.size.x);

float tempColor[3] = {rect.color.r / 255.0f, rect.color.g / 255.0f, rect.color.b / 255.0f};
            if (ImGui::ColorEdit3("Color", tempColor)) {
                rect.color.r = static_cast<unsigned char>(tempColor[0] * 255);
                rect.color.g = static_cast<unsigned char>(tempColor[1] * 255);
                rect.color.b = static_cast<unsigned char>(tempColor[2] * 255);
            }

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

    ImGui::Begin("Play");

    float windowWidth = ImGui::GetWindowSize().x;

    float buttonWidth = 120.0f;

    ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

    if (Application::currentMode == MODE_EDIT) {
        if (ImGui::Button("Play", ImVec2(buttonWidth, 0))) {
            Application::currentMode = MODE_PLAY;
            currentCamera = playCam;
        }
    } else {
        if (ImGui::Button("Stop Playing", ImVec2(buttonWidth, 0))) {
            Application::currentMode = MODE_EDIT;
            currentCamera = editorCam;
        }
    }

    ImGui::End();


    RenderFileManagerPanel("project/", rects, *editorCam, *playCam);

    rlImGuiEnd();

}