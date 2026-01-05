#include "XEngine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "platform/platform.h"
#include "renderer/renderer.h"
#include <iostream>

GLFWwindow* window;

Renderer::Renderer renderer;

namespace XENGINE {
    
    void Init() {

        Platform::CreateWindow("Test", 1200, 800);
        renderer.Init();

    }
    
    bool WindowShouldClose() {
        return Platform::WindowShouldClose();
    }
    
    void StartDrawing() {
        renderer.StartDrawing();
    }
    
    void EndDrawing() {
        Platform::SwapBuffers();
    }
    
    void CloseWindow() {
        glfwDestroyWindow(window);
    }
}
