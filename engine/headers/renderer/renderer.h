#pragma once
#include "shader/shader.h"
#include "texture/texture.h"

namespace Renderer {

class Renderer {

public:  
    void Init();
    void StartDrawing();
    void processInput();
private:
    unsigned int shaderProgram;
    unsigned int VBO, VAO, EBO;
    Shader::Shader* m_DefaultShader;
    Texture::Texture* m_texture;
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
};

}
