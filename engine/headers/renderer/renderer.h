#pragma once
#include "shader/shader.h"
#include "texture/texture.h"
#include "gameObject/gameObject.h"
#include "camera/camera.h"
#include "input/input.h"

namespace Renderer {

class Renderer {

public:  
    void Init();
    void StartDrawing(Shader::Shader* Shader, Camera::Camera camera);
    void processInput();
    GameObject::GameObject gameObject;
    
private:
    unsigned int shaderProgram;
    unsigned int VBO, VAO, EBO;
    Texture::Texture* m_texture;
    // glm::vec3 cameraPos;
    // glm::vec3 cameraFront;
    // glm::vec3 cameraUp;
    
};

}
