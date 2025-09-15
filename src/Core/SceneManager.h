#pragma once

#include <vector>
#include <string>
#include "raylib.h"
#include "Entities/Rectangle.h"
#include "Entities/AreaLight.h"
#include "Entities/SceneObject.h"

class SceneManager {
public:
    static void CreateScene(char* SceneName);

    static void SaveScene(
        const std::vector<RectangleObject>& rectangles,
        const std::vector<LightEntity>& lights,
        const Camera3D& editorCamera,
        const Camera3D& playCamera,
        const std::string& filename);

    static SceneData LoadScene(
        const std::string& filepath,
        Camera3D& editorCamera,
        Camera3D& playCamera);
};