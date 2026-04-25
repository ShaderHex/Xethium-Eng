#include "XEngine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "renderer/renderer.h"
#include "framebuffer/framebuffer.h"
#include "scene/sceneManager.h"
#include <iostream>
#include <vector>

GLFWwindow* window;

Renderer::Renderer renderer;
XENGINE::framebuffer::Framebuffer fb;
XENGINE::sceneManager g_sceneManager;
XENGINE::Scene activeScene;
namespace XENGINE {
    Platform::Input input;
    

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
        activeScene = g_sceneManager.GetCurrentActiveScene();
        fb.Bind();
        renderer.processInput();
        renderer.ClearScreen();
        // for (auto& gameObject : activeScene.GetGameObject().GetCubeObjects()) {
        //     renderer.StartDrawing(Shader, camera, activeScene.GetGameObject(), gameObject);
        // }

        for (auto& gameObject : activeScene.GetGameObject().GetCubeObjects()) {
            renderer.StartDrawing(Shader, camera, activeScene.GetGameObject(), gameObject);
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

    // Scene
    void SwitchActiveScene(XENGINE::Scene& scene) {
        g_sceneManager.SwitchActiveScene(scene);
    }
} 