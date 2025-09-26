#include "Application.h"
#include "Renderer/Renderer.h"
#include "Camera/Camera.h"
#include "SceneManager.h"

Mode Application::currentMode = MODE_EDIT;
Camera3D* currentCamera = nullptr;

bool isRuntimeInit = false;

RectangleObject* GetRectangleByName(std::vector<RectangleObject>& rects, const std::string& name) {
    for (auto& r : rects) {
        if (r.name == name)
            return &r;
    }
    return nullptr;
}

void Application::Init() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    InitWindow(1200, 800, "Xethium");
    rlImGuiSetup(true);

    currentMode = MODE_EDIT;

    EditorCamera::InitCam();
    EditorCamera::InitPlayCam();

    currentCamera = &EditorCamera::editorCamera;

    rlImGuiSetup(true);
    renderer.Init();
    SetTargetFPS(60);
}

bool Application::CurrentGameMode() {
    return currentMode == MODE_EDIT;
}

void Application::Run() {
    while (!WindowShouldClose()) {

        float dt = GetFrameTime();

        if (currentMode == MODE_EDIT) {
            EditorCamera::UpdateEditorCamera();
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (currentMode == MODE_EDIT) {
            isRuntimeInit = false;
            renderer.RenderFrame(*currentCamera, rectangles);
            renderer.ImGuiRender(
                CurrentGameMode(),
                rectangles,
                currentCamera,
                &EditorCamera::editorCamera,
                &EditorCamera::playCamera
            );
        } 
        else if (currentMode == MODE_PLAY) {
            if (!isRuntimeInit) {
                std::cout << "Init Runtime\n";
                renderer.InitRuntime();

                scripting.init();
                scripting.runScriptFile("project/assets/Scripts/game.lua");

                scripting.getState().set_function("MoveRectangleByName", [&](const std::string& name, float x, float y, float z) {
                    RectangleObject* r = GetRectangleByName(rectangles, name);
                    if (r) r->position = { x, y, z };
                });

                scripting.getState().set_function("SetColorByName", [&](const std::string& name, int r, int g, int b, int a) {
                    RectangleObject* rect = GetRectangleByName(Application::rectangles, name);
                    if (rect) rect->color = { (unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a };
                });

                sol::function luaStart = scripting.getState()["Start"];
                if (luaStart.valid()) {
                    luaStart();
                }

                isRuntimeInit = true;
            }

            sol::function luaUpdate = scripting.getState()["Update"];
            if (luaUpdate.valid()) {
                luaUpdate(dt);
            }

            renderer.RenderRuntime(rectangles);
            renderer.ImGuiRenderRuntime(
                CurrentGameMode(),
                rectangles,
                currentCamera,
                &EditorCamera::editorCamera,
                &EditorCamera::playCamera
            );
        }

        EndDrawing();
    }
}

void Application::Shutdown() {
    rlImGuiShutdown();
    CloseWindow();
}