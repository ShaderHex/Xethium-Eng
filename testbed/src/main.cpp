#include "XEngine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>



int main() {
    XENGINE::Init();
    
    while (!XENGINE::WindowShouldClose()) {
        XENGINE::StartDrawing();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        XENGINE::EndDrawing();
    }
    XENGINE::CloseWindow();
}