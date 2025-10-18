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
#include "imgui_impl_raylib.h"

#include "Camera/Camera.h"
#include "Core/FileManager.h"
#include "Core/EngineState.h"
#include "Core/GizmoManager.h"
#include "Core/SkyBox.h"
#include "Core/Console.h"
#include "Entities/Rectangle.h"
#include "Entities/AreaLight.h"
#include "Entities/Sphere.h"
#include "Entities/MeshComponent.h"
#include "Core/MaterialManager.h"
#include "Core/Global.h"

class Renderer {
public:
    void Init(MaterialManager& matManager);
    void DrawSceneObjects(Camera3D& currentCamera, std::vector<RectangleObject>& rects, MaterialManager& matManager);
    void DrawSceneObjectsNoBloom(Camera3D& currentCamera, std::vector<RectangleObject>& rects, MaterialManager& matManager);
    void RenderShadowPass(const LightEntity& light, std::vector<RectangleObject>& rects);
    void RenderFrame(Camera3D& currentCamera, std::vector<RectangleObject>& rects, MaterialManager& matManager);
    void ImGuiRender(bool CanEdit, std::vector<RectangleObject>& rects, Camera3D*& currentCamera, Camera3D* editorCam, Camera3D* playCam, MaterialManager& matManager);
    void HandleInput(std::vector<RectangleObject>& rects, Camera2D camera);
    void ApplyPostProcessing();
    void RenderRuntime(std::vector<RectangleObject>& rects, MaterialManager& matManager);
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
    std::vector<MeshObject> meshes;
    RenderTexture2D playCamView;
    char mesh_dir[256];
    char texture_dir[256];
    int bloomQualityUniformLoc;
    int bloomSampleUniformLoc;
    Vector3 sunDirection;
    Vector3 camPosTarget;
    char skyboxDir[2048];
    EngineMaterial* sharedDefaultMat;
    Image whiteImg;
    Texture2D whiteTex;
    bool isTexFound;
    char matDir[2048];
    bool isScenePropertyOn;
    std::vector<RectangleObject> normalCubeObjects;
};