#include "Application.h"
#include "Renderer/Renderer.h"

enum Mode {
    MODE_PLAY,
    MODE_EDIT
};

Mode currentMode;
Camera2D *currentCamera;

void Application::Init() {
    InitWindow(800, 600, "Xethium");
    currentMode = MODE_EDIT;
    cam.InitCam();
    renderer.Init();

    currentCamera = &camera;
}

void Application::Run() {
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            currentMode = (currentMode == MODE_EDIT) ? MODE_PLAY : MODE_EDIT;
            currentCamera = (currentMode == MODE_EDIT) ? &camera : &playCamera;
        }

        if (currentMode == MODE_EDIT){
            cam.UpdateCamera();
            if (IsKeyPressed(KEY_H)) {
                RectangleObject rect;
                rect.position = { (float)GetRandomValue(0, 500), (float)GetRandomValue(0, 500)};
                rect.size = {100, 100};
                rect.color = (RED);
                
                rectangles.push_back(rect);
                std::cout<<"Pushed back to the memory\n";
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
        ClearBackground(RAYWHITE);

        renderer.RenderFrame(*currentCamera);

        DrawText((currentMode == MODE_EDIT ? "Edit Mode" : "Play Mode"), 10, 10, 20, BLACK);

        EndDrawing();
    }
}


void Application::Shutdown() {
    CloseWindow();
}
