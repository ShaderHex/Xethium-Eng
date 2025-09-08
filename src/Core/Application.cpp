#include "Application.h"
#include "Renderer/Renderer.h"
#include "Camera/Camera.h"
#include "SceneManager.h"

Mode Application::currentMode = MODE_EDIT;
Camera3D* currentCamera = nullptr;

void Application::Init() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    InitWindow(1200, 800, "Xethium");
    rlImGuiSetup(true);

    currentMode = MODE_EDIT;

    EditorCamera::InitCam();
    EditorCamera::InitPlayCam();

    currentCamera = &EditorCamera::editorCamera;

    renderer.Init();

    rectangles = SceneManager::LoadScene("scenes/scene.json",
                                         EditorCamera::editorCamera,
                                         EditorCamera::playCamera);

    scripting.init();
    scripting.runScriptFile("project/assets/Scripts/game.lua");

    sol::function luaStart = scripting.getState()["Start"];
    if (luaStart.valid()) {
        luaStart();
    }
}

bool Application::CurrentGameMode() {
    return currentMode == MODE_EDIT;
}

void Application::Run() {
    while (!WindowShouldClose()) {

        float dt = GetFrameTime();

        if (IsKeyPressed(KEY_SPACE)) {
            currentMode = (currentMode == MODE_EDIT) ? MODE_PLAY : MODE_EDIT;
            currentCamera = (currentMode == MODE_EDIT) ? &EditorCamera::editorCamera
                                                       : &EditorCamera::playCamera;
        }

        if (currentMode == MODE_EDIT) {
            EditorCamera::UpdateEditorCamera();
        }

        sol::function luaUpdate = scripting.getState()["update"];
        if (luaUpdate.valid()) {
            luaUpdate(dt);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        //std::cout << "---------------------------------------------------------------------------------------------------" << std::endl;
        //std::cout << "Rendering the frames..." << std::endl;
        renderer.RenderFrame(*currentCamera, rectangles);
        //std::cout << "Exit rendering frames..." << std::endl;
        //std::cout << "---------------------------------------------------------------------------------------------------" << std::endl;

        //std::cout << "---------------------------------------------------------------------------------------------------" << std::endl;
        //std::cout << "Rendering the ImGui Windows..." << std::endl;
        renderer.ImGuiRender(CurrentGameMode(), rectangles, currentCamera,
                             &EditorCamera::editorCamera, &EditorCamera::playCamera);
        //std::cout << "Exit rendering ImGui Windows..." << std::endl;
        //std::cout << "---------------------------------------------------------------------------------------------------" << std::endl;

        DrawText((currentMode == MODE_EDIT ? "Edit Mode" : "Play Mode"), 10, 10, 20, BLACK);

        EndDrawing();
    }
}


void Application::Shutdown() {
    rlImGuiShutdown();
    CloseWindow();
}