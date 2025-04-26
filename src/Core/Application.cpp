#include "Application.h"
#include "Renderer/Renderer.h"

enum Mode {
    MODE_PLAY,
    MODE_EDIT
};

Mode currentMode;
Camera2D *currentCamera;

void Application::Init() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    InitWindow(800, 600, "Xethium");
    rlImGuiSetup(true);
    currentMode = MODE_EDIT;
    cam.InitCam();
    renderer.Init();

    currentCamera = &camera;

    //rectangles = SceneManager::LoadScene("scenes/scene.json", *currentCamera);
}

bool Application::CurrentGameMode() {
    if (currentMode == MODE_EDIT){
        return true;
    } else {
        return false;
    }
}

void Application::Run() {
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            currentMode = (currentMode == MODE_EDIT) ? MODE_PLAY : MODE_EDIT;
            currentCamera = (currentMode == MODE_EDIT) ? &camera : &playCamera;
        }

        if (currentMode == MODE_EDIT){
            cam.UpdateCamera();
            if (IsKeyPressed(KEY_L)) {
                rectangles = SceneManager::LoadScene("scenes/scene.json", camera, playCamera);
            }
        }

        if (currentMode == MODE_EDIT && IsKeyPressed(KEY_C)) {
            playCamera.target = (Vector2){ 0.0f, 0.0f };
            playCamera.offset = (Vector2){ 0.0f, 0.0f };
            playCamera.rotation = 0.0f;
            playCamera.zoom = 1.0f;
            std::cout << "Play camera created\n";
        }
        
        BeginDrawing();
        
        renderer.RenderFrame(*currentCamera, rectangles);
        renderer.ImGuiRender(CurrentGameMode(), rectangles, *currentCamera);
        DrawText((currentMode == MODE_EDIT ? "Edit Mode" : "Play Mode"), 10, 10, 20, BLACK);

        EndDrawing();
    }
}

void Application::Shutdown() {
    rlImGuiShutdown();
    CloseWindow();
}
