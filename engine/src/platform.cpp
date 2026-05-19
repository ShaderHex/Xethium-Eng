#include "platform/platform.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <iostream>

static GLFWwindow* s_Window = nullptr;
XENGINE::framebuffer::FramebufferSpec m_fbSpec;
XENGINE::framebuffer::Framebuffer m_fb;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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

    glfwSwapInterval(0);
    std::cout << "[Platform] Created window: " << s_Window << "\n";
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

void ChangeNativeWindowTitle(const char* title) {
    glfwSetWindowTitle(s_Window, title);
}

float GetDeltaTime() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    return deltaTime;
}

void UpdateFramebuffer(GLFWwindow* window, int width, int height) {
    m_fbSpec.width = width;
    m_fbSpec.height = height;
    m_fb.Update(m_fbSpec);
    glViewport(0, 0, width, height);
}

void UpdateWindow(XENGINE::framebuffer::Framebuffer& fb, XENGINE::framebuffer::FramebufferSpec& fbSpec) {
    m_fbSpec = fbSpec;
    m_fb = fb;
    // glfwGetWindowSize(s_Window, &fbSpec.width, &fbSpec.height);
    glfwSetFramebufferSizeCallback(s_Window, UpdateFramebuffer);
}

void Shutdown() {
    glfwDestroyWindow(s_Window);
}

}