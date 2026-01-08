#include "XEngine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

int main() {
    XENGINE::Init();
    
    while (!XENGINE::WindowShouldClose()) {
        XENGINE::StartDrawing();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        XENGINE::EndDrawing();
    }
    XENGINE::CloseWindow();
}
