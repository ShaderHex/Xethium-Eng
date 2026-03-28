#include "renderer/renderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb_image.h"
#include "platform/platform.h"
#include "mesh/meshFactory.h"
#include "gameObject/gameObject.h"
#include "input/keycode.h"
#include <iostream>
#include <fstream>

namespace Renderer{

float texCoords[] = {
    0.0f, 0.0f,   // bottom-left
    1.0f, 0.0f,   // bottom-right
    0.5f, 1.0f    // top-center
};


void Renderer::Init() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
    }
    std::cout << "CREATED VBO, VAO, EBO VARIABLES" <<std::endl;

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);
    
    // m_texture = new Texture::Texture("wall.jpg");
    std::cout << "Loaded!\n";
    // m_texture->Bind();
    
    //m_DefaultShader->use();
    //m_DefaultShader->setInt("texture1", 1);
    

}

void Renderer::processInput() {
    const float cameraSpeed = 0.05f;
    // input.Update();
    // if (input.IsActionHeld("bye")) {
    //     std::cout << "bye is held\n";
    // }
}

void Renderer::StartDrawing(Shader::Shader* Shader, Camera::Camera camera, GameObject::GameObject gameObject) {
    glfwPollEvents();

    Shader->use();
    
    glm::mat4 view;
    view = glm::lookAt(camera.position, camera.front + camera.position, camera.up);
    camera.UpdateVectors();
    //Shader.setMat4();
    
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(camera.fov), 1200.0f / 800.0f, camera.nearPlane, camera.farPlane);
    
    int viewLoc = glGetUniformLocation(Shader->ID, "view");
    int projectionLoc = glGetUniformLocation(Shader->ID, "projection");
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    //glUniform1i(glGetUniformLocation(Shader->ID, "texture1"), 0);
    
    gameObject.Render(Shader, m_texture);
    //Shader->setInt("texture1", 1);
    //m_texture->Bind();
}

void Renderer::ClearScreen() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1);
}

}
