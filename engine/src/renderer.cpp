#include "renderer/renderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb_image.h"
#include <iostream>
#include <fstream>

namespace Renderer{

float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

float texCoords[] = {
    0.0f, 0.0f,   // bottom-left
    1.0f, 0.0f,   // bottom-right
    0.5f, 1.0f    // top-center
};

void Renderer::Init() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
    }
    m_DefaultShader = new Shader::Shader("shaders/vertex.vs", "shaders/fragment.fs");
    std::cout << "CREATED VBO, VAO, EBO VARIABLES" <<std::endl;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    std::cout << "CREATED UNIQUE NUMBERS!" <<std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    std::cout << "BINDED THEM!" <<std::endl;

    glGenVertexArrays(1, &VAO);

    // bind vertex array object
    glBindVertexArray(VAO);
    // copy the vertices array in a buffer for opengl to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    m_texture = new Texture::Texture("wall.jpg");
    std::cout << "Loaded!\n";
    m_texture->Bind();
    
    m_DefaultShader->use();
    m_DefaultShader->setInt("texture2", 1);

    }

void Renderer::StartDrawing() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.29, 0.30, 0.41, 1);
    glfwPollEvents();
    m_DefaultShader->use();
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    unsigned int transformLoc = glGetUniformLocation(m_DefaultShader->ID, "transform");

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    glUniform1i(glGetUniformLocation(m_DefaultShader->ID, "texture1"), 0);
    glBindVertexArray(VAO);
}

}
