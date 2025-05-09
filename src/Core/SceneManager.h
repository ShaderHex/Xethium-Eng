#pragma once
#include <vector>
#include <string> 

#include "Entities/Rectangle.h"
#include "UiDgenerator.h"

class SceneManager {
public:
    static void SaveScene(
        const std::vector<RectangleObject>& rectangles, 
        const Camera2D& editorCamera, 
        const Camera2D& playCamera, 
        const std::string& filename
    );
    static std::vector<RectangleObject> LoadScene(
        const std::string& filepath,
        Camera2D& editorCamera,
        Camera2D& playCamera);
    static void CreateScene(char* SceneName);
};