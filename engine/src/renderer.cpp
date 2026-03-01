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

    m_texture = new Texture::Texture("wall.jpg");
    std::cout << "Loaded!\n";
    m_texture->Bind();
    
    //m_DefaultShader->use();
    //m_DefaultShader->setInt("texture1", 1);

    //#################################################
    // CAMERA
    //#################################################
//    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//    
//    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
//    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
//    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
//    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
//    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//    cameraUp = glm::cross(cameraDirection, cameraRight);
    
    glEnable(GL_DEPTH_TEST);
}

void Renderer::processInput() {
    const float cameraSpeed = 0.05f;
    //if(glfwGetKey(Platform::GetNativeWindow(), GLFW_KEY_W) == GLFW_PRESS)
    //    cameraPos += cameraSpeed * cameraFront;
    //if(glfwGetKey(Platform::GetNativeWindow(), GLFW_KEY_S) == GLFW_PRESS)
    //    cameraPos -= cameraSpeed * cameraFront;
    //if(glfwGetKey(Platform::GetNativeWindow(), GLFW_KEY_D) == GLFW_PRESS)
    //    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    //if(glfwGetKey(Platform::GetNativeWindow(), GLFW_KEY_A) == GLFW_PRESS)
    //    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void Renderer::StartDrawing(Shader::Shader* Shader, Camera::Camera camera) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1);
    glfwPollEvents();

    Shader->use();
    
    glm::mat4 view;
    view = glm::lookAt(camera.position, camera.target, camera.up);
    //Shader.setMat4();

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(camera.fov), 1200.0f / 800.0f, camera.nearPlane, camera.farPlane);

    int viewLoc = glGetUniformLocation(Shader->ID, "view");
    int projectionLoc = glGetUniformLocation(Shader->ID, "projection");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glUniform1i(glGetUniformLocation(Shader->ID, "texture1"), 0);

    gameObject.Render(Shader);
}

}
