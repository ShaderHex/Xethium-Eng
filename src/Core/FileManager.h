#include "imgui.h"
#include <filesystem>
#include <string>
#include <vector>
#include <iostream>

class FileManager {
public:
    static void RenderFileManagerPanel(const std::string& projectDir);
};