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

void Renderer::RenderFrame(Camera2D currentCamera, const std::vector<RectangleObject>& rects) {
    ClearBackground(GRAY);
    BeginMode2D(currentCamera);
    for (const auto& r : rects) {
        DrawRectangleV(r.position, r.size, r.color);
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
    if (ImGui::Button("Create Camera")) {
        // Future work: create camera logic
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
    ImGui::End();

    ImGui::Begin("Files");
    ImGui::End();

    RenderFileManagerPanel("project/", rects);

    rlImGuiEnd();
}
