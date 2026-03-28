#include "XEngine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "renderer/renderer.h"
#include <iostream>
#include <vector>

GLFWwindow* window;

Renderer::Renderer renderer;

namespace XENGINE {
    Platform::Input input;
    std::vector<GameObject::GameObject> v_gameObject;

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
        renderer.ClearScreen();
        for (auto& gameObject : v_gameObject) {
            renderer.StartDrawing(Shader, camera, gameObject);
        }
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
    GameObject::GameObject CreateCube(float x, float y, float z, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ, Color color, const char* texturePath) {
        GameObject::GameObject gameObject;

        gameObject.CreateCube(x, y, z, rotX, rotY, rotZ, scaleX, scaleY, scaleZ, color, texturePath);
        v_gameObject.push_back(gameObject);

        return gameObject;
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
