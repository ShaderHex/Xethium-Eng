#include "XEngine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "renderer/renderer.h"
#include <iostream>
#include <vector>
#include "framebuffer/framebuffer.h"

GLFWwindow* window;

Renderer::Renderer renderer;
XENGINE::framebuffer::Framebuffer fb;
namespace XENGINE {
    Platform::Input input;
    std::vector<GameObject::GameObject> v_gameObject;

    // Platform
    void Init(const char *title, int windowX, int windowY) {

        Platform::CreateWindow(title, windowX, windowY);
        renderer.Init();
        XENGINE::framebuffer::FramebufferSpec fbSpec;
        fbSpec.width = windowX;
        fbSpec.height = windowY;
        
        fb.Initialize(fbSpec);
    }
    
    bool WindowShouldClose() {
        return Platform::WindowShouldClose();
    }
    
    void StartDrawing(Shader::Shader* Shader, Camera::Camera camera) {
        fb.Bind();
        renderer.processInput();
        renderer.ClearScreen();
        for (auto& gameObject : v_gameObject) {
            renderer.StartDrawing(Shader, camera, gameObject);
        }
        input.Update();
    }
    
    void EndDrawing() {
        fb.Unbind();
        Platform::SwapBuffers();
    }
    
    void CloseWindow() {
        glfwDestroyWindow(window);
    }

    float GetDeltaTime() {
        return Platform::GetDeltaTime();
    }

    GLFWwindow* GetNativeWindow() {
        // std::cout << "[Engine] Window allocation " << Platform::GetNativeWindow() << "\n";
        return Platform::GetNativeWindow();
    }

    void ChangeNativeWindowTitle(const char* title) {
        return Platform::ChangeNativeWindowTitle(title);
    } 

    // Shader
    Shader::Shader* CreateShader(const char* vertexPath, const char* fragmentPath) {
        return new Shader::Shader(vertexPath, fragmentPath);
    }

    void useShader(void* shader) {
        static_cast<Shader::Shader*>(shader)->use();
    }

    // Game Object
    GameObject::GameObject::Object* CreateCube(float x, float y, float z, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ, Color color) {
        v_gameObject.emplace_back();

        GameObject::GameObject& gameObject = v_gameObject.back();

        gameObject.CreateCube(x, y, z, rotX, rotY, rotZ, scaleX, scaleY, scaleZ, color);

        return &gameObject.m_cubeObjects.back();
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