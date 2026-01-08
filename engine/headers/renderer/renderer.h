#pragma once
#include "shader/shader.h"
#include "texture/texture.h"

namespace Renderer {

class Renderer {

public:  
    void Init();
    void StartDrawing();
private:
    unsigned int shaderProgram;
    unsigned int VBO, VAO, EBO;
    Shader::Shader* m_DefaultShader;
    Texture::Texture* m_texture;
};

}
