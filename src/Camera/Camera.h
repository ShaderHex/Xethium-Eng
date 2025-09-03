// Camera.h
#pragma once
#include "raylib.h"
#include "raymath.h"
#include <string>

class EditorCamera {
public:
    static Camera3D editorCamera;
    static Camera3D playCamera;

    static void InitCam();
    static void InitPlayCam();
    static void UpdateEditorCamera();
    static void UpdatePlayCamera();
    static Camera3D LoadCam(const std::string& filePath);
};
