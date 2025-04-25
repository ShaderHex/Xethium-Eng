#pragma once
#include <vector>
#include <string> 

#include "Entities/Rectangle.h"

class SceneManager {
public:
    static void SaveScene(const std::vector<RectangleObject>& rectangles, const std::string& path);
    static std::vector<RectangleObject> LoadScene(const std::string& path, Camera2D& camera);
};
