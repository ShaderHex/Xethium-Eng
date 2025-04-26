#include "FileManager.h"

namespace fs = std::filesystem;

void FileManager::RenderFileManagerPanel(const std::string& projectDir) {
    try {
        // Check if the project directory exists before attempting to open it
        if (!fs::exists(projectDir) || !fs::is_directory(projectDir)) {
            ImGui::Begin("File Explorer");
            ImGui::Text("Error: Project directory '%s' does not exist!", projectDir.c_str());
            ImGui::End();
            return;
        }

        if (ImGui::Begin("File Explorer")) {
            static std::string currentPath = projectDir;

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
