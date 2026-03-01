#include "XEngine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

int main() {
    XENGINE::Init("testbed", 1200, 800);

    auto shader = XENGINE::CreateShader("shaders/vertex.vs", "shaders/fragment.fs");
    XENGINE::CreateCube(0, 0, 0, 0, 0, 0, 1, 1, 1, {255, 0, 0});
    XENGINE::CreateCube(0, 1, 0, 0, 0, 0, 10, 1, 1, {255, 255, 255});
    auto camera = XENGINE::CreateCamera();

    camera.position = {0.0f, 0.0f, 25.0f};
    float cameraPosSpeed = 0.05f;
    while (!XENGINE::WindowShouldClose()) {

        XENGINE::StartDrawing(shader, camera);

        XENGINE::useShader(shader);
        
        XENGINE::EndDrawing();
    }
    XENGINE::CloseWindow();
}
