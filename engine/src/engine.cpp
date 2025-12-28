#include "XEngine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "platform/platform.h"
#include <iostream>

GLFWwindow* window;

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(1.0f, 0.8f, 0.2f, 1.0f);\n"
    "}\0";

unsigned int shaderProgram;
unsigned int VBO, VAO;

namespace XENGINE {
    
    void Init() {

        Platform::CreateWindow("Test", 1200, 800);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD\n";
        }

            std::cout << "CREATED VBO AND VAO VARIABLES" <<std::endl;
            glGenBuffers(1, &VBO);
            std::cout << "CREATED UNIQUE NUMBERS!" <<std::endl;
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            std::cout << "BINDED THEM!" <<std::endl;
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            std::cout << "GLBUFFERDATA RAN!" <<std::endl;

            glGenVertexArrays(1, &VAO);

            unsigned int vertexShader, fragmentShader;
            
            // Compiling Vertex shader
            std::cout << "COMPILING VERTEX SHADER" <<std::endl;
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
            glCompileShader(vertexShader);
            std::cout << "DONE! COMPILING VERTEX SHADER" <<std::endl;

            
            // Compiling Fragment shader
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(fragmentShader);
            

            ///////////////////
            // Vertex shader compile checking
            ///////////////////
            int  success;
            char infoLog[512];
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

            if(!success)
            {
                glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            }

            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
            if(!success) {
                glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            }
            // bind vertex array object
            glBindVertexArray(VAO);
            // copy the vertices array in a buffer for opengl to use
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            // set our vertex attributes pointers
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
    }
    
    bool WindowShouldClose() {
        return Platform::WindowShouldClose();
    }
    
    void StartDrawing() {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1, 1, 1, 1);
        glfwPollEvents();
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
    }
    
    void EndDrawing() {
        Platform::SwapBuffers();
    }
    
    void CloseWindow() {
        glfwDestroyWindow(window);
    }
}
