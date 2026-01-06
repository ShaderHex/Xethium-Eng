#pragma once
#include "shader/shader.h"

namespace Renderer {

class Renderer {

public:  
    void Init();
    void StartDrawing();
private:
    unsigned int shaderProgram;
    unsigned int VBO, VAO;
    Shader::Shader* defaultShader;
};

}
