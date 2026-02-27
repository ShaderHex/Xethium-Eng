#define BUILDING_XETHIUMLIB
#include "XEngine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

int main() {
    XENGINE::Init("testbed", 1200, 800);

    auto shader = XENGINE::CreateShader("shaders/vertex.vs", "shaders/fragment.fs");
    XENGINE::CreateCube(0, 0, 0, 0, 0, 0, 1, 1, 1, {255, 0, 0});
    XENGINE::CreateCube(0, 1, 0, 0, 0, 0, 10, 1, 1, {255, 255, 255});
    // XENGINE::GameObject::GameObject gameObject;
    
    while (!XENGINE::WindowShouldClose()) {
        XENGINE::StartDrawing(shader);
        XENGINE::useShader(shader);
            
            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        XENGINE::EndDrawing();
    }
    XENGINE::CloseWindow();
}
