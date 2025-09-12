#pragma once

#include <vector>
#include <string>
#include "raylib.h"
#include "Entities/Rectangle.h"

class SceneManager {
public:
    static void CreateScene(char* SceneName);

    static void SaveScene(
        const std::vector<RectangleObject>& rectangles,
        const Camera3D& editorCamera,
        const Camera3D& playCamera,
        const std::string& filename);

    static std::vector<RectangleObject> LoadScene(
        const std::string& filepath,
        Camera3D& editorCamera,
        Camera3D& playCamera);
};