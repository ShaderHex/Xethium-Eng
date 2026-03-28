#include "XEngine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "renderer/renderer.h"
#include <iostream>

GLFWwindow* window;

Renderer::Renderer renderer;

namespace XENGINE {
    Platform::Input input;

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
        input.Update();
    }
    
    void EndDrawing() {
        Platform::SwapBuffers();
    }
    
    void CloseWindow() {
        glfwDestroyWindow(window);
    }

    float GetDeltaTime() {
        return Platform::GetDeltaTime();
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

    // Input
    bool IsActionPressed(std::string action) {
        return input.IsActionPressed(action);
    }

    bool IsActionReleased(std::string action) {
       return input.IsActionReleased(action);
    }

    bool IsActionHeld(std::string action) {
       return input.IsActionHeld(action);
    }

    void CreateAction(std::string action, int key) {
        input.CreateAction(action, key);
    }
} 
