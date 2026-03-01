#include "XEngine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "platform/platform.h"
#include "renderer/renderer.h"
#include <iostream>

GLFWwindow* window;

Renderer::Renderer renderer;

namespace XENGINE {
    // Platform
    void Init(const char *title, int windowX, int windowY) {

        Platform::CreateWindow(title, windowX, windowY);
        renderer.Init();

    }
    
    bool WindowShouldClose() {
        return Platform::WindowShouldClose();
    }
    
    void StartDrawing(Shader::Shader* Shader, Camera::Camera camera) {
        renderer.processInput();
        renderer.StartDrawing(Shader, camera);
    }
    
    void EndDrawing() {
        Platform::SwapBuffers();
    }
    
    void CloseWindow() {
        glfwDestroyWindow(window);
    }

    // Shader
    Shader::Shader* CreateShader(const char* vertexPath, const char* fragmentPath) {
        return new Shader::Shader(vertexPath, fragmentPath);
    }

    void useShader(void* shader) {
        static_cast<Shader::Shader*>(shader)->use();
    }

    // Game Object
    void CreateCube(float x, float y, float z, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ, Color color) {
        renderer.gameObject.CreateCube(x, y, z, rotX, rotY, rotZ, scaleX, scaleY, scaleZ, color);
    }

    // Camera
    Camera::Camera CreateCamera() {
        Camera::Camera camera;
        
        return camera;
    }
} 
