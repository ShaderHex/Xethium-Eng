#pragma once
#include "shader/shader.h"
#include "texture/texture.h"
#include "gameObject/gameObject.h"

namespace Renderer {

class Renderer {

public:  
    void Init();
    void StartDrawing(void* Shader);
    void processInput();
    GameObject::GameObject gameObject;
private:
    unsigned int shaderProgram;
    unsigned int VBO, VAO, EBO;
    Texture::Texture* m_texture;
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    
};

}
