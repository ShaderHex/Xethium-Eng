#include "platform/platform.h"
#include "GLFW/glfw3.h"
#include <iostream>

static GLFWwindow* s_Window = nullptr;

namespace Platform {

void CreateWindow(const char* title, int width, int height) {
    if(!glfwInit()) {
        std::cout<<"Failed to init GLFW"<<std::endl;
    }
        
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
    s_Window = glfwCreateWindow(width, height, title, NULL, NULL);
    if(!s_Window) {
        std::cout<<"Failed to create window!";
    }
    
    glfwMakeContextCurrent(s_Window);
}

bool WindowShouldClose() {
    return glfwWindowShouldClose(s_Window);
}

void PollEvents() {
    glfwPollEvents();
}

void SwapBuffers() {
    glfwSwapBuffers(s_Window);
}

GLFWwindow* GetNativeWindow() {
    return s_Window;
}

void Shutdown() {
    glfwDestroyWindow(s_Window);
}

}
