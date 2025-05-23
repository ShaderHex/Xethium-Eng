#include "Renderer.h"
#include "Core/Application.h"

namespace fs = std::filesystem;

// These are used to remember where we are and which file is selected
std::string currentPathWrite;
std::string fileNameWrite;

void Renderer::Init() {
    // Renderer-specific initialization
}

void RenderFileManagerPanel(const std::string& projectDir, std::vector<RectangleObject>& rects) {
    try {
        if (!fs::exists(projectDir) || !fs::is_directory(projectDir)) {
            ImGui::Begin("File Explorer");
            ImGui::Text("Error: Project directory '%s' does not exist!", projectDir.c_str());
            ImGui::End();
            return;
        }

        if (ImGui::Begin("File Explorer")) {
            static std::string currentPath = projectDir; // Start from projectDir

            ImGui::Text("Current Path: %s", currentPath.c_str());

            if (fs::exists(currentPath) && fs::is_directory(currentPath)) {
                for (const auto& entry : fs::directory_iterator(currentPath)) {
                    const std::string fileName = entry.path().filename().string();

                    if (entry.is_directory()) {
                        if (ImGui::Button(("Open Folder: " + fileName).c_str())) {
                            currentPath = entry.path().string();
                        }
                    } else {
                        if (ImGui::Button(("Select File: " + fileName).c_str())) {
                            std::cout << "File selected: " << entry.path().string() << std::endl;
                            rects = SceneManager::LoadScene(entry.path().string(), camera, playCamera);
                            std::cout << "Loaded: " << entry.path().string() << std::endl;
                            fileNameWrite = fileName; // ONLY set fileNameWrite when clicked
                            currentPathWrite = currentPath; // Also update path when file clicked
                        }
                    }
                }
            } else {
                ImGui::Text("Error: Current path is invalid!");
            }

            ImGui::End();
        }
    } catch (const std::exception& e) {
        std::cerr << "An error occurred while rendering the File Manager: " << e.what() << std::endl;
        ImGui::Begin("File Explorer");
        ImGui::Text("An error occurred while loading the directory.");
        ImGui::End();
    }
}

void Renderer::RenderFrame(Camera2D currentCamera, std::vector<RectangleObject>& rects) {
    ClearBackground(GRAY);
    BeginMode2D(currentCamera);

    Vector2 mousePos = GetMousePosition();
    Vector2 worldMouse = GetScreenToWorld2D(mousePos, currentCamera);

    hoveredUiD = -1;

    for (auto& r : rects) {
        Rectangle rect = { r.position.x, r.position.y, r.size.x, r.size.y };
    
        DrawRectangleV(r.position, r.size, r.color);
    
        if (CheckCollisionPointRec(worldMouse, rect)) {
            hoveredUiD = r.UiD;
            DrawRectangleLinesEx(rect, 2, YELLOW);
            std::cout << "Mouse hovering on UiD: " << r.UiD << std::endl;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (hoveredUiD != -1) {
                    selectedUiD = hoveredUiD;
                    DrawRectangleLinesEx(rect, 2, WHITE);
                }
            }
        }
    
        if (selectedUiD == r.UiD) {
            DrawRectangleLinesEx(rect, 2, WHITE);
        }
    }
    
    EndMode2D();
}


void Renderer::ImGuiRender(bool CanEdit, std::vector<RectangleObject>& rects, Camera2D currentCamera) {
    rlImGuiBegin();

    ImGui::Begin("Inspector");
    if (ImGui::Button("Create")) {
        if (CanEdit) {
            rectang.CreateRect(rects);
        }
    }

    static int selectedUiD = -1;
    if (ImGui::Button("Select Rectangle to Modify")) {
        ImGui::OpenPopup("Select UiD");
    }
    if (ImGui::BeginPopup("Select UiD")) {
        for (const auto& rect : rects) {
            if (ImGui::Selectable(("UiD: " + std::to_string(rect.UiD)).c_str())) {
                selectedUiD = rect.UiD; // Store selected UiD
            }
        }
        for (const auto& rect : rects) {
            selectedUiD = rect.UiD; // Store selected UiD

        }
        ImGui::EndPopup();
    }

    // Modify selected rectangle's properties based on its UiD
    if (selectedUiD != -1) {
        for (auto& rect : rects) {
            if (rect.UiD == selectedUiD) {
                ImGui::Text("Modify Rectangle: UiD %d", selectedUiD);

                // Modify Position
                ImGui::InputFloat2("Position", (float*)&rect.position);

                // Modify Size
                ImGui::InputFloat2("Size", (float*)&rect.size);

                // Modify Color
                ImGui::ColorEdit3("Color", (float*)&rect.color);
            }
        }
    }


    if (ImGui::Button("Create Camera")) {
        // TODO: create camera logic
    }
    if (ImGui::Button("Load Scene")) {
        if (CanEdit) {
            std::cout << "Loaded rectangles: " << rects.size() << std::endl;
        }
    }
    if (ImGui::Button("Save Scene")) {
        if (CanEdit) {
            if (!fileNameWrite.empty()) {
                std::string fullPath = currentPathWrite + "/" + fileNameWrite;
                SceneManager::SaveScene(rects, camera, playCamera, fullPath);
                std::cout << "Saved to: " << fullPath << std::endl;
            } else {
                std::cout << "No file selected to save!\n";
            }
        }
    }

    if (ImGui::Button("Create Scene")) {
        ImGui::OpenPopup("Create Scene Popup");
    }
    
    if (ImGui::BeginPopupModal("Create Scene Popup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char SceneName[128] = "";
    
        ImGui::InputText("Name", SceneName, IM_ARRAYSIZE(SceneName));
    
        if (ImGui::Button("OK", ImVec2(120, 0))) {
            if (strlen(SceneName) > 0) {
                SceneManager::CreateScene(SceneName);
            } else {
                std::cerr << "[UI] Error: Scene name cannot be empty!\n";
            }
            ImGui::CloseCurrentPopup();
            SceneName[0] = '\0';
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
    
        ImGui::EndPopup();
    }
    

    ImGui::End();

    RenderFileManagerPanel("project/", rects);

    rlImGuiEnd();
}