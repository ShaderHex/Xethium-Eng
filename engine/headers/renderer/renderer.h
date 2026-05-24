#pragma once
#include "shader/shader.h"
#include "texture/texture.h"
#include "gameObject/gameObject.h"
#include "camera/camera.h"
#include "input/input.h"
#include "scene/scene.h"
#include "resourceManager/resourceManager.h"
#include "framebuffer/framebuffer.h"


namespace Renderer {

class Renderer {

public:  
    void Init(XENGINE::framebuffer::FramebufferSpec fbSpec);
    void Draw(XENGINE::Scene activeScene, Shader::Shader* Shader);
    void StartDrawing(Shader::Shader* Shader, Camera::Camera camera, XENGINE::Scene activeScene);
    void DrawQuadMesh(GLuint textureID);
    void ClearScreen();
private:
    unsigned int shaderProgram;
    unsigned int VBO, VAO, EBO;
    Texture::Texture* m_texture;
    std::shared_ptr<Texture::Texture> m_textureColorbuffer;
    std::shared_ptr<Shader::Shader> m_framebufferShader;
    // glm::vec3 cameraPos;
    // glm::vec3 cameraFront;
    // glm::vec3 cameraUp;
    
};

}
