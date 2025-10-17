#include "Renderer.h"
#include "Core/Application.h"
#include "Core/SceneManager.h"
#include "imgui.h"
#include "rlImGui.h"
#include "imgui_internal.h"
#include "ImGuizmo.h"
#include <raygizmo.h>

#include <filesystem>
#include <iostream>
#include <cstring>
#include <cfloat>

int Renderer::selectedLightUiD = -1;
LightSystem Renderer::lightSystem;

namespace fs = std::filesystem;

int hoveredUiD = -1;
int selectedUiD = -1;
int CubeNumber;
int SphereNumber;

std::string currentPathWrite;
std::string fileNameWrite;
std::unordered_set<long long> usedUiDs;

Vector3 playCamPos = EditorCamera::playCamera.position;

Skybox skybox;

RenderTexture2D target;
const char* str = "1";
void DrawEngineGrid(int slices = 1000, float spacing = 1.0f) {
    int halfSlices = slices / 2;

    rlBegin(RL_LINES);
    for (int i = -halfSlices; i <= halfSlices; i++) {
        if (i == 0)
            rlColor3f(1.0f, 0.0f, 0.0f); 
        else if (i % 10 == 0)
            rlColor3f(1.0f, 1.0f, 1.0f); 
        else
            rlColor3f(0.6f, 0.6f, 0.6f); 

        rlVertex3f((float)i*spacing, 0.0f, (float)-halfSlices*spacing);
        rlVertex3f((float)i*spacing, 0.0f, (float)halfSlices*spacing);

        rlVertex3f((float)-halfSlices*spacing, 0.0f, (float)i*spacing);
        rlVertex3f((float)halfSlices*spacing, 0.0f, (float)i*spacing);
    }
    rlEnd();
}

void imguiTheme();

static RenderTexture2D LoadRenderTextureDepthTex(int width, int height) {
    RenderTexture2D target = { 0 };

    target.id = rlLoadFramebuffer();

    if (target.id > 0) {
        rlEnableFramebuffer(target.id);

        target.texture.id = rlLoadTexture(0, width, height, PIXELFORMAT_UNCOMPRESSED_R32, 1);
        target.texture.width = width;
        target.texture.height = height;
        target.texture.format = PIXELFORMAT_UNCOMPRESSED_R32;
        target.texture.mipmaps = 1;

        target.depth.id = rlLoadTextureDepth(width, height, false);
        target.depth.width = width;
        target.depth.height = height;
        target.depth.format = 19;
        target.depth.mipmaps = 1;

        rlFramebufferAttach(target.id, target.texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

        if (rlFramebufferComplete(target.id)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);

        rlDisableFramebuffer();
    } else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");

    return target;
}

void Renderer::Init(MaterialManager& matManager) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    font = ImGui::GetIO().Fonts->AddFontFromFileTTF("Roboto.ttf", 18.0f);
    if (!font) {
        CloseWindow();
    }
    IM_ASSERT(font != nullptr);
    ImGui_ImplRaylib_BuildFontAtlas();
    io.FontDefault = font;

    postShader = LoadShader(0, "project/shaders/postProcessing.fs");
    if (postShader.id == 0) {
        std::cout << "Post-processing shader failed to load!" << std::endl;
    } else {
        std::cout << "Post-processing shader loaded!\n";
    }

    strcpy(skyboxDir, "project/assets/skybox/default/default.hdr");
    skybox.Load(skyboxDir, true);
    target = LoadRenderTexture(1200, 800);
    
    shader = LoadShader("project/shaders/lighting.vs", "project/shaders/lighting.fs");
    if (shader.id == 0) {
        std::cout << "Lighting shader failed to load!" << std::endl;
    } else {
        std::cout << "Lighting shader loaded!\n";
    }
    
    RedirectCoutToLogger();

    Renderer::lightSystem.SetShader(&shader);
    
    ShadowMap = LoadRenderTextureDepthTex(2048, 2048);

    depthShadowMap = ShadowMap.depth;

    Shader depthShader = LoadShader(0, TextFormat("project/shaders/depth_render.fs", 330));
    int depthLoc = GetShaderLocation(depthShader, "depthTexture");
    int flipTextureLoc = GetShaderLocation(depthShader, "flipY");
    SetShaderValue(depthShader, flipTextureLoc, str, SHADER_UNIFORM_INT);
    
    if (ShadowMap.id == 0 || ShadowMap.texture.id == 0) {
        std::cout << "Failed to create shadow map render texture!" << std::endl;
    } else {
        std::cout << "Shadow map created successfully. ID: " << ShadowMap.texture.id << std::endl;
    }

    Mesh cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);
    depthShader = LoadShader("project/shaders/shadowmap.vs", "project/shaders/shadowmap.fs");
    lightVPLoc = GetShaderLocation(depthShader, "lightVP");
    shadowMapLoc = GetShaderLocation(depthShader, "shadowMap");
    
    shadowShader = LoadShader(TextFormat("project/shaders/shadowmap.vs", 3.3),
                                     TextFormat("project/shaders/shadowmap.fs", 3.3));
    shadowShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shadowShader, "viewPos");
    lightDir = Vector3Normalize(Vector3{ 0.35f, -1.0f, -0.35f });
    Color lightColor = WHITE;
    Vector4 lightColorNormalized = ColorNormalize(lightColor);
    lightDirLoc = GetShaderLocation(shadowShader, "lightDir");
    int lightColLoc = GetShaderLocation(shadowShader, "lightColor");
    SetShaderValue(shadowShader, lightDirLoc, &lightDir, SHADER_UNIFORM_VEC3);
    SetShaderValue(shadowShader, lightColLoc, &lightColorNormalized, SHADER_UNIFORM_VEC4);
    int ambientLoc = GetShaderLocation(shadowShader, "ambient");
    float ambient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    SetShaderValue(shadowShader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);
    int lightVPLoc = GetShaderLocation(shadowShader, "lightVP");
    int shadowMapLoc = GetShaderLocation(shadowShader, "shadowMap");
    int shadowMapResolution = 1024;
    SetShaderValue(shadowShader, GetShaderLocation(shadowShader, "shadowMapResolution"), &shadowMapResolution, SHADER_UNIFORM_INT);
    cube = LoadModelFromMesh(cubeMesh);
    cube.materials[0].shader = shadowShader;
    SetTextureFilter(ShadowMap.texture, RL_TEXTURE_FILTER_LINEAR);

    lightCamera = { 0 };
    lightCamera.position = Vector3Scale(lightDir, -15.0f);
    lightCamera.target = Vector3Zero();
    lightCamera.projection = CAMERA_ORTHOGRAPHIC;
    lightCamera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    lightCamera.fovy = 20.0f;

    lightView = { 0 };
    lightProj = { 0 };
    lightViewProj = { 0 };
    textureActiveSlot = 10;
    UploadMesh(&cubeMesh, false);
    cubeMaterial = LoadMaterialDefault();

    bloomShader = LoadShader(0, "project/shaders/bloom.fs");
    if(bloomShader.id == 0) {
        printf("Failed to load bloomShader\n");
    } else {
        printf("bloomShader loaded!\n");
    }
    bloomTarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    sceneTarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    playCamView = LoadRenderTexture(256, 256);
    //bloomQualityUniformLoc = GetShaderLocation(bloomShader, "quality");
    //bloomSampleUniformLoc = GetShaderLocation(bloomShader, "sample");
    sunDirection = { -0.3f, -1.0f, -0.2f };

    whiteImg = GenImageColor(1, 1, WHITE);
    whiteTex = LoadTextureFromImage(whiteImg);
    UnloadImage(whiteImg);

    sharedDefaultMat = matManager.GetByName("DefaultWhite");
    if (!sharedDefaultMat) {
        sharedDefaultMat = matManager.CreateMaterial("DefaultWhite", "", WHITE);
        sharedDefaultMat->albedoTexture = whiteTex;
        sharedDefaultMat->isLoaded = true;
    }
    isTexFound = false;
}

bool CheckCollisionRayBox(Ray ray, BoundingBox box) {
    float tmin = (box.min.x - ray.position.x) / ray.direction.x;
    float tmax = (box.max.x - ray.position.x) / ray.direction.x;
    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (box.min.y - ray.position.y) / ray.direction.y;
    float tymax = (box.max.y - ray.position.y) / ray.direction.y;
    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax)) return false;
    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    float tzmin = (box.min.z - ray.position.z) / ray.direction.z;
    float tzmax = (box.max.z - ray.position.z) / ray.direction.z;
    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax)) return false;
    return true;
}

void RenderFileManagerPanel(const std::string& projectDir, std::vector<RectangleObject>& rects, Camera3D& camera, Camera3D& playCamera) {
    try {
        if (!fs::exists(projectDir) || !fs::is_directory(projectDir)) {
            ImGui::Begin("File Explorer");
            ImGui::Text("Error: Project directory '%s' does not exist!", projectDir.c_str());
            ImGui::End();
            return;
        }

        if (ImGui::Begin("File Explorer")) {
            static std::string currentPath = projectDir;
            ImGui::Text("Current Path: %s", currentPath.c_str());

            for (const auto& entry : fs::directory_iterator(currentPath)) {
                const std::string fileName = entry.path().filename().string();

                if (entry.is_directory()) {
                    if (ImGui::Button(("Open Folder: " + fileName).c_str())) {
                        currentPath = entry.path().string();
                    }
                } else {
                    if (ImGui::Button(("Select File: " + fileName).c_str())) {
                        rects.clear();
                        Renderer::lightSystem.lights.clear();
                        
                        SceneData sceneData = SceneManager::LoadScene(entry.path().string(), camera, playCamera);
                        
                        for (const auto& rect : sceneData.rectangles) {
                            rects.push_back(rect);
                        }
                        
                        for (const auto& light : sceneData.lights) {
                            Renderer::lightSystem.lights.push_back(light);
                        }
                    }
                }
            }

        }
        ImGui::End();
    } catch (const std::exception& e) {
        ImGui::Begin("File Explorer");
        ImGui::Text("Error: %s", e.what());
        ImGui::End();
    }
}

void Renderer::DrawSceneObjects(Camera3D& currentCamera, std::vector<RectangleObject>& rects, MaterialManager& matManager) {
    for (auto& r : rects) {
EngineMaterial* mat = matManager.GetById(r.materialID);

if (!mat || mat->albedoTexture.id == 0) {
    cube.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = whiteTex;
    DrawModelEx(cube, r.position, Vector3{0,1,0}, 0.0f, r.size, WHITE);
} else {
    cube.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = mat->albedoTexture;
    DrawModelEx(cube, r.position, Vector3{0,1,0}, 0.0f, r.size, mat->albedoColor);
}


    }

    Ray ray = GetMouseRay(GetMousePosition(), currentCamera);

    if (!ImGui::GetIO().WantCaptureMouse) {
        for (auto& r : rects) {
            BoundingBox box = { r.position - r.size / 2, r.position + r.size / 2 };
            if (CheckCollisionRayBox(ray, box)) {
                hoveredUiD = r.UiD;
                DrawCubeWires(r.position, r.size.x, r.size.y, r.size.z, RED);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    selectedUiD = r.UiD;
                    Renderer::selectedLightUiD = -1;
                }
            }
        }
    }

    if (Application::currentMode == MODE_EDIT && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        float closestDist = FLT_MAX;
        for (auto& light : Renderer::lightSystem.lights) {
            if (!light.enabled) continue;

            Vector3 diff = Vector3Subtract(light.position, ray.position);
            float t = Vector3DotProduct(diff, ray.direction);
            Vector3 closestPoint = Vector3Add(ray.position, Vector3Scale(ray.direction, t));
            float dist = Vector3Distance(closestPoint, light.position);

            if (dist < 0.3f && t > 0) {
                if (t < closestDist) {
                    closestDist = t;
                    Renderer::selectedLightUiD = light.UiD;
                    selectedUiD = -1;
                }
            }
        }
    }

    

    for (auto& s : sphere) {
        Matrix transform = MatrixTranslate(s.position.x, s.position.y, s.position.z);
        SetShaderValueMatrix(shader, GetShaderLocation(shader, "matModel"), transform);

        Matrix matNormal = MatrixTranspose(MatrixInvert(transform));
        SetShaderValueMatrix(shader, GetShaderLocation(shader, "matNormal"), matNormal);

        DrawSphere(s.position, s.radius, s.color); 

        if (Application::currentMode == MODE_EDIT) {
            Vector3 half = { s.radius, s.radius, s.radius };
            BoundingBox box = {
                Vector3Subtract(s.position, half),
                Vector3Add(s.position, half)
            };

            if (CheckCollisionRayBox(ray, box)) {
                hoveredUiD = s.UiD;
                DrawSphereWires(s.position, s.radius, 16, 16, YELLOW);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    selectedUiD = s.UiD;
                    Renderer::selectedLightUiD = -1;
                }
            }

            if (selectedUiD == s.UiD) {
                DrawSphereWires(s.position, s.radius, 16, 16, WHITE);
            }
        }
    }

for (auto& m : meshes) {
        if (!m.isLoaded) m.Load();

        Matrix transform = MatrixIdentity();
        transform = MatrixMultiply(transform, MatrixRotateXYZ(Vector3{
            DEG2RAD * m.rotation.x,
            DEG2RAD * m.rotation.y,
            DEG2RAD * m.rotation.z
        }));
        transform = MatrixMultiply(transform, MatrixScale(m.scale.x, m.scale.y, m.scale.z));
        transform = MatrixMultiply(transform, MatrixTranslate(m.position.x, m.position.y, m.position.z));

        DrawModel(m.model, m.position, 1.0, m.color);

        DrawSphere(m.position, 0.05f, RED);

        if (Application::currentMode == MODE_EDIT) {
            BoundingBox box = GetModelBoundingBox(m.model);
            BoundingBox transformedBox = {
                Vector3Add(box.min, m.position),
                Vector3Add(box.max, m.position)
            };
            
            Ray ray = GetMouseRay(GetMousePosition(), currentCamera);
            if (CheckCollisionRayBox(ray, transformedBox)) {
                DrawBoundingBox(transformedBox, YELLOW);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    selectedUiD = m.UiD;
                }
            }

            if (selectedUiD == m.UiD) {
                static Transform gizmoTransform = GizmoIdentity();
                gizmoTransform.translation = m.position;

                gizmoTransform.rotation = QuaternionFromEuler(
                    DEG2RAD * m.rotation.x,
                    DEG2RAD * m.rotation.y,
                    DEG2RAD * m.rotation.z
                );

                gizmoTransform.scale = m.scale;

                DrawGizmo3D(GIZMO_TRANSLATE |  GIZMO_SCALE, &gizmoTransform);

                m.position = gizmoTransform.translation;

                Vector3 euler = QuaternionToEuler(gizmoTransform.rotation);
                m.rotation = Vector3Scale(euler, RAD2DEG);

                m.scale = gizmoTransform.scale;
            }

        }
    }
}


void Renderer::RenderShadowPass(const LightEntity& light, std::vector<RectangleObject>& rects) {
    lightCam = {0};
    
    if (light.type == 0) {
        lightCam.position = light.position;
        lightCam.target = light.target;
        lightCam.up = {0.0f, 1.0f, 0.0f};
        lightCam.fovy = 90.0f;
        lightCam.projection = CAMERA_ORTHOGRAPHIC;
    } else {
        lightCam.position = light.position;
        lightCam.target = Vector3Add(light.position, Vector3Normalize(Vector3Subtract(light.target, light.position)));
        lightCam.up = {0.0f, 1.0f, 0.0f};
        lightCam.fovy = 90.0f;
        lightCam.projection = CAMERA_PERSPECTIVE;
    }

    BeginTextureMode(ShadowMap);
        ClearBackground(WHITE);
        BeginMode3D(lightCam);
            lightView = rlGetMatrixModelview();
            lightProj = rlGetMatrixProjection();
            for (auto& light : Renderer::lightSystem.lights) {
                Renderer::lightSystem.UpdateLightMatrix(light, 1024, 1024);
            }

            for (auto& r : rects) {
                DrawCube(r.position, r.size.x, r.size.y, r.size.z, WHITE);
            }
            for (auto& s : sphere) {
                DrawSphere(s.position, s.radius, WHITE);
            }
        EndMode3D();
    EndTextureMode();
}

void Renderer::RenderFrame(Camera3D& currentCamera, std::vector<RectangleObject>& rects, MaterialManager& matManager) {
    if (IsWindowResized()) {
        UnloadRenderTexture(target);
        UnloadRenderTexture(bloomTarget);
        target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        bloomTarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        sceneTarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    }

    Vector3 cameraPos = currentCamera.position;
    SetShaderValue(shadowShader, shadowShader.locs[SHADER_LOC_VECTOR_VIEW], &cameraPos, SHADER_UNIFORM_VEC3);
    
    lightDir = Vector3Normalize(sunDirection);
    lightCamera.position = Vector3Scale(lightDir, -15.0f);
    SetShaderValue(shadowShader, lightDirLoc, &lightDir, SHADER_UNIFORM_VEC3);

    lightView = MatrixLookAt(lightCamera.position, camPosTarget, (Vector3){0, 1, 0});
    lightProj = MatrixOrtho(-150, 150, -150, 150, 1.0f, 400.0f);
    lightViewProj = MatrixMultiply(lightProj, lightView);

    //SetShaderValue(bloomShader, bloomSampleUniformLoc, "100.0", SHADER_UNIFORM_FLOAT);
    
    BeginTextureMode(ShadowMap);
            ClearBackground(WHITE);
            
            BeginMode3D(lightCamera);
            lightView = rlGetMatrixModelview();
            lightProj = rlGetMatrixProjection();

            DrawSceneObjects(currentCamera, rects, matManager);
            EndMode3D();
            
            EndTextureMode();
            lightViewProj = MatrixMultiply(lightView, lightProj);
            BeginMode3D(currentCamera);
            
            EndMode3D();
            BeginTextureMode(sceneTarget);
            ClearBackground(BLACK);
            rlDisableDepthTest();
            skybox.Draw(currentCamera);
            rlEnableDepthTest();
    SetShaderValueMatrix(shadowShader, lightVPLoc, lightViewProj);
    rlEnableShader(shadowShader.id);
    
    rlActiveTextureSlot(textureActiveSlot);
    rlEnableTexture(ShadowMap.depth.id);
    rlSetUniform(shadowMapLoc, &textureActiveSlot, SHADER_UNIFORM_INT, 1);
    BeginMode3D(currentCamera);

        DrawEngineGrid();

        if (shader.id != 0) {
            BeginShaderMode(shader);

            int shadowMapLoc = GetShaderLocation(shader, "shadowMap");
            int textureSlot = 10;
            rlActiveTextureSlot(textureSlot);
            rlEnableTexture(ShadowMap.depth.id);
            SetShaderValue(shader, shadowMapLoc, &textureSlot, SHADER_UNIFORM_INT);

            for (auto& light : Renderer::lightSystem.lights) {
                Matrix lightVP = Renderer::lightSystem.GetLightMatrix(light, 512, 512);
                int lightVPLoc = GetShaderLocation(shader, "lightVP");
                SetShaderValueMatrix(shader, lightVPLoc, lightVP);
            }

            DrawSceneObjects(currentCamera, rects, matManager);

            Ray ray = GetMouseRay(GetMousePosition(), currentCamera);
            hoveredUiD = -1;

            if (Application::currentMode == MODE_EDIT && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                float closestDist = FLT_MAX;
                for (auto& light : Renderer::lightSystem.lights) {
                    if (!light.enabled) continue;

                    Vector3 diff = Vector3Subtract(light.position, ray.position);
                    float t = Vector3DotProduct(diff, ray.direction);
                    Vector3 closestPoint = Vector3Add(ray.position, Vector3Scale(ray.direction, t));
                    float dist = Vector3Distance(closestPoint, light.position);

                    if (dist < 0.3f && t > 0) {
                        if (t < closestDist) {
                            closestDist = t;
                            Renderer::selectedLightUiD = light.UiD;
                            selectedUiD = -1;
                        }
                    }
                }
            }

            if (Application::currentMode == MODE_EDIT) {
                DrawCube(EditorCamera::playCamera.position, 1, 1, 1, BLUE);
            }

            Renderer::lightSystem.UpdateLights(currentCamera);

            EndShaderMode();
        }

        Renderer::lightSystem.DebugDraw();

        if (Application::currentMode == MODE_EDIT && Renderer::selectedLightUiD != -1) {
            LightEntity* selectedLight = Renderer::lightSystem.GetLightByUiD(Renderer::selectedLightUiD);
            if (selectedLight) {
                static Transform gizmoTransform = GizmoIdentity();
                gizmoTransform.translation = selectedLight->position;

                DrawGizmo3D(GIZMO_TRANSLATE, &gizmoTransform);
                selectedLight->position = gizmoTransform.translation;

                DrawSphereWires(selectedLight->position, 0.25f, 12, 12, YELLOW);
            }
        }

        for (auto& r : rects) {
            if (selectedUiD == r.UiD) {
                static Transform gizmoTransform = GizmoIdentity();
                gizmoTransform.translation = r.position;
                gizmoTransform.scale = r.size;
                DrawGizmo3D(GIZMO_TRANSLATE | GIZMO_SCALE, &gizmoTransform);
                r.position = gizmoTransform.translation;
                r.size = gizmoTransform.scale;
            }
        }

        for (auto& s : sphere) {
            if (selectedUiD == s.UiD) {
                Transform gizmoTransform = GizmoIdentity();
                gizmoTransform.translation = s.position;
                DrawGizmo3D(GIZMO_TRANSLATE | GIZMO_ROTATE, &gizmoTransform);
                s.position = gizmoTransform.translation;
            }
        }

        EndMode3D();
    EndTextureMode();

    BeginTextureMode(target);
        ClearBackground(BLACK);
            BeginMode3D(currentCamera);
                rlDisableDepthTest();
                
                skybox.Draw(currentCamera);
                rlEnableDepthTest();

                DrawTextureRec(
                    sceneTarget.texture,
                    Rectangle{ 0, 0, (float)sceneTarget.texture.width, -(float)sceneTarget.texture.height },
                    Vector2{ 0, 0 },
                    WHITE
                );

            EndMode3D();
    EndTextureMode();

    BeginTextureMode(bloomTarget);
    ClearBackground(BLACK);
    BeginShaderMode(bloomShader);


        DrawTextureRec(
            sceneTarget.texture,
            Rectangle{ 0, 0, (float)sceneTarget.texture.width, (float)sceneTarget.texture.height },
            Vector2{ 0, 0 },
            WHITE
        );

    EndShaderMode();
    EndTextureMode();

    ApplyPostProcessing();
    
}

void Renderer::ApplyPostProcessing() {
    BeginShaderMode(postShader);
        DrawTextureRec(
            target.texture,
            {0, 0, (float)target.texture.width, -(float)target.texture.height},
            {0, 0},
            WHITE
        );

        rlSetBlendMode(BLEND_ADDITIVE);
        DrawTextureRec(bloomTarget.texture,
            Rectangle{0, 0, (float)bloomTarget.texture.width, (float)bloomTarget.texture.height},
            Vector2{0, 0}, WHITE);
        rlSetBlendMode(BLEND_ALPHA);

    EndShaderMode();
}

static void GetCameraMatrices(Camera3D& cam, float view[16], float proj[16]) {
    Matrix viewMat = GetCameraMatrix(cam);
    Matrix projMat = MatrixPerspective(cam.fovy * DEG2RAD, (float)GetScreenWidth() / GetScreenHeight(), 0.01f, 1000.0f);
    memcpy(view, &viewMat, sizeof(float) * 16);
    memcpy(proj, &projMat, sizeof(float) * 16);
}

long long GenerateUniqueUiD() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long long> dis(1, 999999999999999);

    long long newUiD;

    do {
        newUiD = dis(gen);
    } while (usedUiDs.count(newUiD));

    usedUiDs.insert(newUiD);

    return newUiD;
}

void Renderer::ImGuiRender(bool CanEdit, std::vector<RectangleObject>& rects, Camera3D*& currentCamera, Camera3D* editorCam, Camera3D* playCam, MaterialManager& matManager) {
    rlImGuiBegin();
    imguiTheme();
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()), ImGuiCond_Always);

    ImGuiWindowFlags dockspaceFlags = ImGuiWindowFlags_NoTitleBar |
                                     ImGuiWindowFlags_NoResize |
                                     ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoCollapse |
                                     ImGuiWindowFlags_NoBringToFrontOnFocus |
                                     ImGuiWindowFlags_NoNavFocus |
                                     ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0,0,0,0));

    ImGui::Begin("DockSpace", nullptr, dockspaceFlags);

    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor();

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0,0), ImGuiDockNodeFlags_PassthruCentralNode);

    static bool docked = false;
    if (!docked) {
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

        ImGuiID top_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.08f, nullptr, &dockspace_id);
        ImGuiID right_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, nullptr, &dockspace_id);
        ImGuiID left_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.25f, nullptr, &dockspace_id);
        ImGuiID bottom_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.30f, nullptr, &dockspace_id);
        ImGuiID bottom_right_id = ImGui::DockBuilderSplitNode(bottom_id, ImGuiDir_Right, 0.5f, &bottom_id, nullptr);

        ImGui::DockBuilderDockWindow("Play", top_id);
        ImGui::DockBuilderDockWindow("Inspector", right_id);
        ImGui::DockBuilderDockWindow("File Explorer", bottom_id);
        ImGui::DockBuilderDockWindow("Lights", right_id);
        ImGui::DockBuilderDockWindow("Console", bottom_id);
        ImGui::DockBuilderDockWindow("Shadow Map Debug", bottom_right_id);
        ImGui::DockBuilderDockWindow("Scene Hierarchy", left_id);


        ImGui::DockBuilderFinish(dockspace_id);
        docked = true;
    }

    ImGui::End();

    ImGui::Begin("Inspector");

    if (ImGui::Button("Create Cube") && CanEdit) {
        RectangleObject obj;
        obj.position = {0.0f, 1.0f, 0.0f};
        obj.size = {1.0f, 1.0f, 1.0f};
        obj.UiD = GenerateUniqueUiD();
        obj.name = "Cube" + std::to_string(CubeNumber);
        CubeNumber++;
    
        obj.materialID = sharedDefaultMat->id;

        rects.push_back(obj);
        std::cout << "Created cube with name: " << obj.name << std::endl;
    }

    if (ImGui::Button("Create Sphere") && CanEdit) {
        SphereObject obj;
        obj.position = {0.0f, 1.0f, 0.0f};
        obj.radius = {1};
        obj.color = RED;
        obj.UiD = GenerateUniqueUiD();
        obj.name = "Sphere" + std::to_string(SphereNumber);
        SphereNumber++;
        sphere.push_back(obj);
        std::cout << "Created Sphere with name: " << obj.name << std::endl;
    }

    if (ImGui::Button("Create Mesh")) {
        MeshObject obj("project/assets/models/sponza_gltf/scene.gltf", {0,1,0});
        obj.Load();
        meshes.push_back(obj);
        std::cout << "Created Mesh: " << obj.name << std::endl;
    }

    if (ImGui::Button("Select Object")) ImGui::OpenPopup("Select UiD");

    if (ImGui::BeginPopup("Select UiD")) {
        for (const auto& rect : rects) {
            if (ImGui::Selectable(("Cube UiD: " + std::to_string(rect.UiD)).c_str()))
                selectedUiD = rect.UiD;
                std::cout << "Cube UiD " << rect.UiD << '\n';
        }

        for (const auto& spheres : sphere) {
            if (ImGui::Selectable(("UiD: " + std::to_string(spheres.UiD)).c_str()))
                selectedUiD = spheres.UiD;
            
            std::cout << "Sphere UiD " << spheres.UiD << '\n';
        }
        ImGui::EndPopup();
    }

    for (auto& rect : rects) {
        if (rect.UiD == selectedUiD) {

            if(ImGui::CollapsingHeader("Properties")) {
                ImGui::Text("Modify Object: %d", rect.UiD);
                ImGui::DragFloat3("Position", &rect.position.x);
                ImGui::DragFloat3("Size", &rect.size.x);
            }

            if (ImGui::CollapsingHeader("Material")) {
                EngineMaterial* mat = matManager.GetById(rect.materialID);

                if (mat) {
                    ImGui::Text("Material: %s", mat->name.c_str());
                    ImGui::Separator();

                    float tempColor[3] = {
                        mat->albedoColor.r / 255.0f,
                        mat->albedoColor.g / 255.0f,
                        mat->albedoColor.b / 255.0f
                    };

                    if (ImGui::ColorEdit3("Albedo Color", tempColor)) {
                        mat->albedoColor = {
                            static_cast<unsigned char>(tempColor[0] * 255),
                            static_cast<unsigned char>(tempColor[1] * 255),
                            static_cast<unsigned char>(tempColor[2] * 255),
                            255
                        };
                    }

                    ImGui::SliderFloat("Roughness", &mat->roughness, 0.0f, 1.0f);
                    ImGui::SliderFloat("Metallic", &mat->metallic, 0.0f, 1.0f);

                    static char textureDir[256];
                    strncpy(textureDir, mat->albedoPath.c_str(), sizeof(textureDir));

                    if (ImGui::InputText("Texture Path", textureDir, sizeof(textureDir))) {
                        mat->albedoPath = textureDir;
                    }

                    if (ImGui::Button("Reload Texture")) {
                        if (!mat->albedoPath.empty()) {
                            if (mat->isLoaded) UnloadTexture(mat->albedoTexture);

                            Image img = LoadImage(mat->albedoPath.c_str());
                            if (img.data) {
                                mat->albedoTexture = LoadTextureFromImage(img);
                                UnloadImage(img);
                                mat->isLoaded = true;
                            } else {
                                Image fallback = GenImageColor(1, 1, WHITE);
                                mat->albedoTexture = LoadTextureFromImage(fallback);
                                UnloadImage(fallback);
                                mat->isLoaded = true;
                                std::cout << "WARNING: File not found, using fallback white texture.\n";
                            }
                        } else {
                            if (mat->isLoaded) UnloadTexture(mat->albedoTexture);
                            Image fallback = GenImageColor(1, 1, WHITE);
                            mat->albedoTexture = LoadTextureFromImage(fallback);
                            UnloadImage(fallback);
                            mat->isLoaded = true;
                        }
                    }

                    ImGui::InputText("Material Path", matDir, sizeof(matDir));

                    if(ImGui::Button("Load Material")) {
                        unsigned int matID = matManager.LoadMaterialFromFile(matDir);
                        if (matID != 0) {
                            rect.materialID = matID;
                        } else {
                            rect.materialID = 0;
                        }
                    }

                    if (ImGui::Button("Save Material")) {
                        matManager.SaveToFile(*mat);
                    }
                }
                else {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "No material assigned.");
                }

                if (ImGui::Button("Create & Assign Material")) {
                    EngineMaterial* newMat = matManager.CreateMaterial(
                        "NewMaterial",
                        "",
                        WHITE
                    );

                    if (newMat) {
                        rect.materialID = newMat->id;
                        std::cout << "Assigned material " << newMat->name
                                << " to cube UiD " << rect.UiD << std::endl;
                    }
                }
            }

        }
    }


    for (auto& s : sphere) {
        if (s.UiD == selectedUiD) {
            static GizmoManager gizmo;
            gizmo.transform.translation = s.position;

            gizmo.Draw(GIZMO_TRANSLATE | GIZMO_ROTATE | GIZMO_SCALE);

            Matrix m = gizmo.GetMatrix();
            s.position.x = m.m12;
            s.position.y = m.m13;
            s.position.z = m.m14;


            ImGui::Text("Modify Object: %d", s.UiD);
            ImGui::DragFloat3("Position", &s.position.x);

            float tempColor[3] = {s.color.r / 255.0f, s.color.g / 255.0f, s.color.b / 255.0f};
            if (ImGui::ColorEdit3("Color", tempColor)) {
                s.color.r = static_cast<unsigned char>(tempColor[0] * 255);
                s.color.g = static_cast<unsigned char>(tempColor[1] * 255);
                s.color.b = static_cast<unsigned char>(tempColor[2] * 255);
            }

            if (CanEdit) {
                float view[16], proj[16], objectMatrix[16];
                GetCameraMatrices(*editorCam, view, proj);

                Matrix m = MatrixTranslate(s.position.x, s.position.y, s.position.z);
                memcpy(objectMatrix, &m, sizeof(float) * 16);

                ImGuizmo::SetRect(0, 0, (float)GetScreenWidth(), (float)GetScreenHeight());
                ImGuizmo::SetDrawlist();
                ImGuizmo::Manipulate(view, proj, ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, objectMatrix);

                if (ImGuizmo::IsUsing()) {
                    s.position.x = objectMatrix[12];
                    s.position.y = objectMatrix[13];
                    s.position.z = objectMatrix[14];
                }
            }
        }
    }

    for (auto& m : meshes) {
        if (m.UiD == selectedUiD) {
            ImGui::Text("Modify Mesh: %s (UiD: %d)", m.name.c_str(), m.UiD);
            ImGui::DragFloat3("Position", &m.position.x, 0.1f);
            ImGui::DragFloat3("Rotation", &m.rotation.x, 0.1f);
            ImGui::DragFloat3("Scale", &m.scale.x, 0.1f);
            ImGui::InputText("Mesh Dir", mesh_dir, IM_ARRAYSIZE(mesh_dir));

            float tempColor[3] = { m.color.r / 255.f, m.color.g / 255.f, m.color.b / 255.f };
            if (ImGui::ColorEdit3("Color", tempColor)) {
                m.color.r = static_cast<unsigned char>(tempColor[0]*255);
                m.color.g = static_cast<unsigned char>(tempColor[1]*255);
                m.color.b = static_cast<unsigned char>(tempColor[2]*255);
            }

            if (ImGui::Button("Reload Model")) m.Reload();
        }
    }

    for (auto& light : Renderer::lightSystem.lights) {
        if (light.UiD == Renderer::selectedLightUiD) {
            ImGui::Text("Editing Light: %s", light.name.c_str());
            
            ImGui::Checkbox("Enabled", &light.enabled);
            ImGui::InputText("Name", &light.name[0], 128);
            ImGui::DragFloat3("Position", &light.position.x, 0.1f);
            
            if (light.type == XE_LIGHT_DIRECTIONAL) {
                ImGui::DragFloat3("Direction", &light.target.x, 0.1f);
            }
            
            float colorFloat[3] = {
                light.color.r / 255.0f,
                light.color.g / 255.0f,
                light.color.b / 255.0f
            };
            
            if (ImGui::ColorEdit3("Color", colorFloat)) {
                light.color.r = static_cast<unsigned char>(colorFloat[0] * 255);
                light.color.g = static_cast<unsigned char>(colorFloat[1] * 255);
                light.color.b = static_cast<unsigned char>(colorFloat[2] * 255);
            }
            
            ImGui::DragFloat("Intensity", &light.intensity, 0.1f, 0.0f, 10.0f);
            
            const char* lightTypes[] = { "Directional", "Point" };
            ImGui::Combo("Type", &light.type, lightTypes, 2);
            
            if (ImGui::Button("Delete Light")) {
                for (auto it = Renderer::lightSystem.lights.begin(); it != Renderer::lightSystem.lights.end(); ++it) {
                    if (it->UiD == Renderer::selectedLightUiD) {
                        Renderer::lightSystem.lights.erase(it);
                        Renderer::selectedLightUiD = -1;
                        break;
                    }
                }
            }
        }
    }


    if (ImGui::Button("Save Scene") && !fileNameWrite.empty()) {
        SceneManager::SaveScene(rects, Renderer::lightSystem.lights, *editorCam, *playCam, currentPathWrite + "/" + fileNameWrite);
        std::cout << "Saved to: " << currentPathWrite + "/" + fileNameWrite << std::endl;
    }

    if (ImGui::Button("Create Scene")) ImGui::OpenPopup("Create Scene Popup");

    if (ImGui::BeginPopupModal("Create Scene Popup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char SceneName[128] = "";
        ImGui::InputText("Name", SceneName, IM_ARRAYSIZE(SceneName));

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            if (strlen(SceneName) > 0) SceneManager::CreateScene(SceneName);
            else std::cerr << "[UI] Error: Scene name cannot be empty!\n";

            ImGui::CloseCurrentPopup();
            SceneName[0] = '\0';
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }

    ImGui::End();

    ImGui::Begin("Lights");
    
    if (ImGui::Button("Create Light") && CanEdit) {
        Renderer::lightSystem.CreateDefaultLight(ShadowMap);
    }
    
    if (ImGui::Button("Select Light")) ImGui::OpenPopup("Select Light UiD");
    
    if (ImGui::BeginPopup("Select Light UiD")) {
        for (const auto& light : Renderer::lightSystem.lights) {
            if (ImGui::Selectable((light.name + " (UiD: " + std::to_string(light.UiD) + ")").c_str())) {
                Renderer::selectedLightUiD = light.UiD;
                selectedUiD = -1;
            }
        }
        ImGui::EndPopup();
    }
    
    
    ImGui::End();

    ImGui::Begin("Play");

    float windowWidth = ImGui::GetWindowSize().x;
    float buttonWidth = 120.0f;
    ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

    if (Application::currentMode == MODE_EDIT) {
        if (ImGui::Button("Play", ImVec2(buttonWidth, 0))) {
            //Renderer::editorStateLights = Renderer::lightSystem.lights;
            editorStateRectangles = rects;
            editorStateSphere = sphere;

            Application::currentMode = MODE_PLAY;
            currentCamera = playCam;
        }
    }

    ImGui::End();

    if (ImGui::Begin("Shadow Map Debug")) {
        ImVec2 winSize = ImGui::GetContentRegionAvail();
        
        if (ShadowMap.id != 0 && ShadowMap.texture.id != 0) {
            rlImGuiImage(&ShadowMap.texture);
            
            ImGui::Text("Shadow Map Size: %dx%d", ShadowMap.texture.width, ShadowMap.texture.height);
            ImGui::Text("Texture ID: %u", ShadowMap.texture.id);
            
            if (!Renderer::lightSystem.lights.empty()) {
                LightEntity& light = Renderer::lightSystem.lights[0];
                ImGui::Text("Light Position: (%.2f, %.2f, %.2f)", 
                           light.position.x, light.position.y, light.position.z);
                ImGui::Text("Light Target: (%.2f, %.2f, %.2f)", 
                           light.target.x, light.target.y, light.target.z);
                ImGui::Text("Light Type: %s", light.type == 0 ? "Directional" : "Point");
            }
        } else {
            ImGui::Text("Shadow map not properly initialized!");
            if (ShadowMap.id == 0) {
                ImGui::Text("RenderTexture ID is 0");
            }
            if (ShadowMap.texture.id == 0) {
                ImGui::Text("Texture ID is 0");
            }
        }
    }
    ImGui::End();

    if(ImGui::Begin("Camera View")) {
        rlImGuiImage(&playCamView.texture);
    }
    ImGui::End();

    ImGui::Begin("Scene Hierarchy");

    for (auto& obj : rects)
    {
        ImGuiTreeNodeFlags flags = 0;
        if (obj.UiD == selectedUiD) flags |= ImGuiTreeNodeFlags_Selected;

        bool node_open = ImGui::TreeNodeEx(
            (void*)(intptr_t)obj.UiD,
            flags,
            "%s", obj.name.c_str()
        );

        if (ImGui::IsItemClicked())
        {
            selectedUiD = obj.UiD;
        }

        if (node_open)
        {
            ImGui::TreePop();
        }
    }

    for (auto& obj : Renderer::lightSystem.lights)
    {
        ImGuiTreeNodeFlags flags = 0;
        if (obj.UiD == selectedLightUiD) flags |= ImGuiTreeNodeFlags_Selected;

        bool node_open = ImGui::TreeNodeEx(
            (void*)(intptr_t)obj.UiD,
            flags,
            "%s", obj.name.c_str()
        );

        if (ImGui::IsItemClicked())
        {
            selectedUiD = selectedLightUiD;
        }

        if (node_open)
        {
            ImGui::TreePop();
        }
    }

    ImGui::End();

    ImGui::Begin("Scene Settings");
        ImGui::Text("Skybox directory");
        ImGui::InputText("SkyboxDir", skyboxDir, sizeof(skyboxDir));
        if (ImGui::Button("ReloadSkybox")) {
            skybox.Reload(skyboxDir, true);
        }
    ImGui::End();


    g_Logger.DrawWindow();
    RenderFileManagerPanel("project/", rects, *editorCam, *playCam);

    rlImGuiEnd();
}

void Renderer::InitRuntime() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    font = ImGui::GetIO().Fonts->AddFontFromFileTTF("Roboto.ttf", 18.0f);
    if (!font) {
        CloseWindow();
    }
    IM_ASSERT(font != nullptr);
    ImGui_ImplRaylib_BuildFontAtlas();
    io.FontDefault = font;

    postShader = LoadShader(0, "project/shaders/postProcessing.fs");
    if (postShader.id == 0) {
        std::cout << "Post-processing shader failed to load!" << std::endl;
    } else {
        std::cout << "Post-processing shader loaded!\n";
    }

    skybox.Load("project/assets/skybox/default/default.hdr", true);
    target = LoadRenderTexture(1200, 800);
    
    shader = LoadShader("project/shaders/lighting.vs", "project/shaders/lighting.fs");
    if (shader.id == 0) {
        std::cout << "Lighting shader failed to load!" << std::endl;
    } else {
        std::cout << "Lighting shader loaded!\n";
    }
    
    Renderer::lightSystem.SetShader(&shader);

    UnloadRenderTexture(target);
    target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    RedirectCoutToLogger();
    Renderer::lightSystem.SetShader(&shader);
    
    ShadowMap = LoadRenderTextureDepthTex(2048, 2048);

    depthShadowMap = ShadowMap.depth;

    Shader depthShader = LoadShader(0, TextFormat("project/shaders/depth_render.fs", 330));
    int depthLoc = GetShaderLocation(depthShader, "depthTexture");
    int flipTextureLoc = GetShaderLocation(depthShader, "flipY");
    SetShaderValue(depthShader, flipTextureLoc, str, SHADER_UNIFORM_INT);
    
    if (ShadowMap.id == 0 || ShadowMap.texture.id == 0) {
        std::cout << "Failed to create shadow map render texture!" << std::endl;
    } else {
        std::cout << "Shadow map created successfully. ID: " << ShadowMap.texture.id << std::endl;
    }

    Mesh cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);
    depthShader = LoadShader("project/shaders/shadowmap.vs", "project/shaders/shadowmap.fs");
    lightVPLoc = GetShaderLocation(depthShader, "lightVP");
    shadowMapLoc = GetShaderLocation(depthShader, "shadowMap");
    
    shadowShader = LoadShader(TextFormat("project/shaders/shadowmap.vs", 3.3),
                                     TextFormat("project/shaders/shadowmap.fs", 3.3));
    shadowShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shadowShader, "viewPos");
    lightDir = Vector3Normalize(Vector3{ 0.35f, -1.0f, -0.35f });
    Color lightColor = WHITE;
    Vector4 lightColorNormalized = ColorNormalize(lightColor);
    lightDirLoc = GetShaderLocation(shadowShader, "lightDir");
    int lightColLoc = GetShaderLocation(shadowShader, "lightColor");
    SetShaderValue(shadowShader, lightDirLoc, &lightDir, SHADER_UNIFORM_VEC3);
    SetShaderValue(shadowShader, lightColLoc, &lightColorNormalized, SHADER_UNIFORM_VEC4);
    int ambientLoc = GetShaderLocation(shadowShader, "ambient");
    float ambient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    SetShaderValue(shadowShader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);
    int lightVPLoc = GetShaderLocation(shadowShader, "lightVP");
    int shadowMapLoc = GetShaderLocation(shadowShader, "shadowMap");
    int shadowMapResolution = 1024;
    SetShaderValue(shadowShader, GetShaderLocation(shadowShader, "shadowMapResolution"), &shadowMapResolution, SHADER_UNIFORM_INT);
    cube = LoadModelFromMesh(cubeMesh);
    cube.materials[0].shader = shadowShader;

    lightCamera = { 0 };
    lightCamera.position = Vector3Scale(lightDir, -15.0f);
    lightCamera.target = Vector3Zero();
    lightCamera.projection = CAMERA_ORTHOGRAPHIC;
    lightCamera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    lightCamera.fovy = 20.0f;

    lightView = { 0 };
    lightProj = { 0 };
    lightViewProj = { 0 };
    textureActiveSlot = 10;
    UploadMesh(&cubeMesh, false);
    cubeMaterial = LoadMaterialDefault();

    bloomShader = LoadShader(0, "project/shaders/bloom.fs");
    if(bloomShader.id == 0) {
        printf("Failed to load bloomShader\n");
    } else {
        printf("bloomShader loaded!\n");
    }
    bloomTarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    sceneTarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

}

void Renderer::RenderRuntime(std::vector<RectangleObject>& rects, MaterialManager& matManager) {
    if (IsWindowResized()) {
        UnloadRenderTexture(target);
        UnloadRenderTexture(bloomTarget);
        target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        bloomTarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        sceneTarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    }


    Vector3 cameraPos = EditorCamera::playCamera.position;
    SetShaderValue(shadowShader, shadowShader.locs[SHADER_LOC_VECTOR_VIEW], &cameraPos, SHADER_UNIFORM_VEC3);
    
    lightDir = Vector3Normalize(lightDir);
    lightCamera.position = Vector3Scale(lightDir, -15.0f);
    SetShaderValue(shadowShader, lightDirLoc, &lightDir, SHADER_UNIFORM_VEC3);

    BeginTextureMode(ShadowMap);
            ClearBackground(WHITE);
            
            BeginMode3D(lightCamera);
                lightView = rlGetMatrixModelview();
                lightProj = rlGetMatrixProjection();
                DrawSceneObjects(EditorCamera::playCamera, rects, matManager);
            EndMode3D();
            
    EndTextureMode();
    lightViewProj = MatrixMultiply(lightView, lightProj);

    BeginTextureMode(sceneTarget);
        ClearBackground(BLACK);
        SetShaderValueMatrix(shadowShader, lightVPLoc, lightViewProj);
        rlEnableShader(shadowShader.id);
            
        rlActiveTextureSlot(textureActiveSlot);
        rlEnableTexture(ShadowMap.depth.id);
        rlSetUniform(shadowMapLoc, &textureActiveSlot, SHADER_UNIFORM_INT, 1);
        
        BeginMode3D(EditorCamera::playCamera);

        if (shader.id != 0) {
            BeginShaderMode(shader);

            int shadowMapLoc = GetShaderLocation(shader, "shadowMap");
            int textureSlot = 10;
            rlActiveTextureSlot(textureSlot);
            rlEnableTexture(ShadowMap.depth.id);
            SetShaderValue(shader, shadowMapLoc, &textureSlot, SHADER_UNIFORM_INT);

            for (auto& light : Renderer::lightSystem.lights) {
                Matrix lightVP = Renderer::lightSystem.GetLightMatrix(light, 512, 512);
                int lightVPLoc = GetShaderLocation(shader, "lightVP");
                SetShaderValueMatrix(shader, lightVPLoc, lightVP);
            }

            DrawSceneObjects(EditorCamera::playCamera, rects, matManager);

            Renderer::lightSystem.UpdateLights(EditorCamera::playCamera);

            EndShaderMode();
        }
                
        EndMode3D();
    EndTextureMode();

    BeginTextureMode(target);
        ClearBackground(BLACK);
            BeginMode3D(EditorCamera::playCamera);
                rlDisableDepthTest();
                skybox.Draw(EditorCamera::playCamera);
                rlEnableDepthTest();

                DrawTextureRec(
                    sceneTarget.texture,
                    Rectangle{ 0, 0, (float)sceneTarget.texture.width, -(float)sceneTarget.texture.height },
                    Vector2{ 0, 0 },
                    WHITE
                );

            EndMode3D();
    EndTextureMode();

    BeginTextureMode(bloomTarget);
    ClearBackground(BLACK);
    BeginShaderMode(bloomShader);


        DrawTextureRec(
            sceneTarget.texture,
            Rectangle{ 0, 0, (float)sceneTarget.texture.width, (float)sceneTarget.texture.height },
            Vector2{ 0, 0 },
            WHITE
        );

    EndShaderMode();
    EndTextureMode();

    ApplyPostProcessing();
}

void Renderer::ImGuiRenderRuntime(bool CanEdit, std::vector<RectangleObject>& rects, Camera3D*& currentCamera, Camera3D* editorCam, Camera3D* playCam) {
    rlImGuiBegin();
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()), ImGuiCond_Always);

    ImGuiWindowFlags dockspaceFlags = ImGuiWindowFlags_NoTitleBar |
                                     ImGuiWindowFlags_NoResize |
                                     ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoCollapse |
                                     ImGuiWindowFlags_NoBringToFrontOnFocus |
                                     ImGuiWindowFlags_NoNavFocus |
                                     ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0,0,0,0));

    ImGui::Begin("DockSpace", nullptr, dockspaceFlags);

    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor();

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0,0), ImGuiDockNodeFlags_PassthruCentralNode);

    static bool docked = false;
    if (!docked) {
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

        ImGuiID top_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.08f, nullptr, &dockspace_id);
        ImGuiID right_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, nullptr, &dockspace_id);
        ImGuiID bottom_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.30f, nullptr, &dockspace_id);
        ImGuiID left_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.25f, nullptr, &dockspace_id);

        ImGui::DockBuilderDockWindow("Play", top_id);
        ImGui::DockBuilderDockWindow("Inspector", right_id);
        ImGui::DockBuilderDockWindow("File Explorer", bottom_id);
        ImGui::DockBuilderDockWindow("Lights", right_id);
        ImGui::DockBuilderDockWindow("Console", bottom_id);
        ImGui::DockBuilderDockWindow("Scene Hierarchy", left_id);

        ImGui::DockBuilderFinish(dockspace_id);
        docked = true;
    }

    ImGui::End();


    ImGui::Begin("Inspector");    

    ImGui::End();
    ImGui::Begin("Lights");
    
    ImGui::End();

    ImGui::Begin("Play");

    float windowWidth = ImGui::GetWindowSize().x;
    float buttonWidth = 120.0f;
    ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

    if (Application::currentMode == MODE_EDIT) {
        if (ImGui::Button("Play", ImVec2(buttonWidth, 0))) {
            Application::currentMode = MODE_PLAY;
            currentCamera = playCam;
        }
    } else {
        if (ImGui::Button("Stop Playing", ImVec2(buttonWidth, 0))) {

            //Renderer::lightSystem.lights = Renderer::editorStateLights;
            rects = editorStateRectangles;
            sphere = editorStateSphere;

            Application::currentMode = MODE_EDIT;
            currentCamera = editorCam;
        }
    }

    ImGui::End();

    if (ImGui::Begin("Shadow Map Debug")) {
        ImVec2 winSize = ImGui::GetContentRegionAvail();
        
        if (ShadowMap.id != 0 && ShadowMap.texture.id != 0) {
            rlImGuiImage(&ShadowMap.texture);
            
            ImGui::Text("Shadow Map Size: %dx%d", ShadowMap.texture.width, ShadowMap.texture.height);
            ImGui::Text("Texture ID: %u", ShadowMap.texture.id);
            
            if (!Renderer::lightSystem.lights.empty()) {
                LightEntity& light = Renderer::lightSystem.lights[0];
                ImGui::Text("Light Position: (%.2f, %.2f, %.2f)", 
                           light.position.x, light.position.y, light.position.z);
                ImGui::Text("Light Target: (%.2f, %.2f, %.2f)", 
                           light.target.x, light.target.y, light.target.z);
                ImGui::Text("Light Type: %s", light.type == 0 ? "Directional" : "Point");
            }
        } else {
            ImGui::Text("Shadow map not properly initialized!");
            if (ShadowMap.id == 0) {
                ImGui::Text("RenderTexture ID is 0");
            }
            if (ShadowMap.texture.id == 0) {
                ImGui::Text("Texture ID is 0");
            }
        }
    }
    ImGui::End();

    ImGui::Begin("Scene Hierarchy");

    for (auto& obj : rects)
    {
        ImGuiTreeNodeFlags flags = 0;
        if (obj.UiD == selectedUiD) flags |= ImGuiTreeNodeFlags_Selected;

        bool node_open = ImGui::TreeNodeEx(
            (void*)(intptr_t)obj.UiD,
            flags,
            "%s", obj.name.c_str()
        );

        if (ImGui::IsItemClicked())
        {
            selectedUiD = obj.UiD;
        }

        if (node_open)
        {
            ImGui::TreePop();
        }
    }

    for (auto& obj : Renderer::lightSystem.lights)
    {
        ImGuiTreeNodeFlags flags = 0;
        if (obj.UiD == selectedLightUiD) flags |= ImGuiTreeNodeFlags_Selected;

        bool node_open = ImGui::TreeNodeEx(
            (void*)(intptr_t)obj.UiD,
            flags,
            "%s", obj.name.c_str()
        );

        if (ImGui::IsItemClicked())
        {
            selectedUiD = selectedLightUiD;
        }

        if (node_open)
        {
            ImGui::TreePop();
        }
    }

    ImGui::End();

    g_Logger.DrawWindow();

    rlImGuiEnd();
}

void imguiTheme() {
    ImGuiStyle &style = ImGui::GetStyle();
    ImVec4 *colors = style.Colors;

    style.WindowRounding = 5.0f;
    style.FrameRounding = 5.0f;
    style.ScrollbarRounding = 5.0f;
    style.GrabRounding = 5.0f;
    style.TabRounding = 5.0f;
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.PopupRounding = 5.0f;

    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.f);
    colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);

    colors[ImGuiCol_CheckMark] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f); 
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f); 
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);

    style.FramePadding = ImVec2(8.0f, 4.0f);
    style.ItemSpacing = ImVec2(8.0f, 4.0f);
    style.IndentSpacing = 20.0f;
    style.ScrollbarSize = 16.0f;
}