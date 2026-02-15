
#define BUILDING_XETHIUMLIB
#include "XEngine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

int main() {
    XENGINE::Init("Test", 1200, 800);

    auto shader = XENGINE::CreateShader("shaders/vertex.vs", "shaders/fragment.fs");
    // XENGINE::GameObject::GameObject gameObject;
    XENGINE::CreateCube(0, 0, 0);
    
    while (!XENGINE::WindowShouldClose()) {
        XENGINE::StartDrawing(shader);
            XENGINE::useShader(shader);
            
            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        XENGINE::EndDrawing();
    }
    XENGINE::CloseWindow();
}
