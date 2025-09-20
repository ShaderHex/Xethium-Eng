#pragma once
#define STBI_INCLUDE_EXR
#define RLIGHTS_IMPLEMENTATION

#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <random>

#include "raylib.h"
#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"
#include "ImGuizmo.h"
#include "rlgl.h"
#include "rlights.h"
#include "nlohmann/json.hpp"

#include "Camera/Camera.h"
#include "Entities/Rectangle.h"
#include "Core/FileManager.h"
#include "Core/EngineState.h"
#include "Core/GizmoManager.h"
#include "Core/SkyBox.h"
#include "Entities/AreaLight.h"
#include "Entities/Sphere.h"

class Renderer {
public:
    void Init();
    void RenderFrame(Camera3D& currentCamera, std::vector<RectangleObject>& rects);
    void ImGuiRender(bool CanEdit, std::vector<RectangleObject>& rects, Camera3D*& currentCamera, Camera3D* editorCam, Camera3D* playCam);
    void HandleInput(std::vector<RectangleObject>& rects, Camera2D camera);
    void ApplyPostProcessing();
    void RenderRuntime(std::vector<RectangleObject>& rects);
    void InitRuntime();
    void ImGuiRenderRuntime(bool CanEdit, std::vector<RectangleObject>& rects, Camera3D*& currentCamera, Camera3D* editorCam, Camera3D* playCam);

    int hoveredUiD = -1;
    int selectedUiD = -1;
    bool isDragging = false;
    Vector2 dragOffset = {0, 0};
    static int selectedLightUiD;
    static LightSystem lightSystem;

private:
    EditorCamera cam;
    RectangleObj rectang;
    FileManager filemanager;
    Shader shader;
    Shader postShader;
    using json = nlohmann::json;

    std::vector<LightSystem> lights;
    std::vector<SphereObject> sphere;

};