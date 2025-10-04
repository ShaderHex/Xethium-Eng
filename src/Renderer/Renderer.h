#pragma once
#define STBI_INCLUDE_EXR
#define RLIGHTS_IMPLEMENTATION

#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <random>
#include <stdio.h>

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
#include "Core/Console.h"
#include "imgui_impl_raylib.h"

class Renderer {
public:
    void Init();
    void DrawSceneObjects(Camera3D& currentCamera, std::vector<RectangleObject>& rects);
    void RenderShadowPass(const LightEntity& light, std::vector<RectangleObject>& rects);
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
    RenderTexture2D ShadowMap;
    Texture2D depthShadowMap;
    Shader depthShader;

private:
    EditorCamera cam;
    RectangleObj rectang;
    FileManager filemanager;
    Shader shader;
    Shader postShader;
    Shader bloomShader;
    using json = nlohmann::json;

    std::vector<LightSystem> lights;
    std::vector<SphereObject> sphere;

    std::vector<LightSystem> editorStateLights;
    std::vector<SphereObject> editorStateSphere;
    std::vector<RectangleObject> editorStateRectangles;
    Texture2D dummy = {0};
    int lightVPLoc;
    int shadowMapLoc;
    int textureSlot;
    Vector3 lightDir;
    Shader shadowShader;
    int lightDirLoc;
    Camera3D lightCam;
    Matrix lightView;
    Matrix lightProj;
    Matrix lightViewProj;
    int textureActiveSlot;
    Model cube;
    Camera3D lightCamera;
    Mesh cubeMesh;
    Material cubeMaterial;
    bool isPlayMode;
    RenderTexture2D bloomTarget;
    RenderTexture2D sceneTarget;
    ImFont* font;
};