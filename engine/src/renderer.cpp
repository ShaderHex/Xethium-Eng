#include "renderer/renderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <fstream>

namespace Renderer{

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

void Renderer::Init() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
    }
    defaultShader = new Shader::Shader("shaders/vertex.vs", "shaders/fragment.fs");
    std::cout << "CREATED VBO AND VAO VARIABLES" <<std::endl;
    glGenBuffers(1, &VBO);
    std::cout << "CREATED UNIQUE NUMBERS!" <<std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    std::cout << "BINDED THEM!" <<std::endl;

    glGenVertexArrays(1, &VAO);

            // bind vertex array object
    glBindVertexArray(VAO);
            // copy the vertices array in a buffer for opengl to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            // set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}

void Renderer::StartDrawing() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.29, 0.30, 0.41, 1);
    glfwPollEvents();
    defaultShader->use();
    glBindVertexArray(VAO);
}

}
