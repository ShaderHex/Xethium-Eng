// Probably future me, Please organize the code
// as the time of writing this, move the engine initializer specifically the renderer init to the renderer namespace and class also the shaderProgram and VAO, VBO
// also the triangle matrices and possibly make the shaders compile from glsl and not from variable anymore
// that'd be it, thanks in advance!
  
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
