#include "XEngine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "renderer/renderer.h"
#include "framebuffer/framebuffer.h"
#include "scene/sceneManager.h"
#include "ecs/componentStorage.h"
#include <iostream>
#include <vector>

GLFWwindow* window;

Renderer::Renderer renderer;
XENGINE::framebuffer::Framebuffer fb;
XENGINE::sceneManager g_sceneManager;
XENGINE::Scene* activeScene = nullptr;
XENGINE::framebuffer::FramebufferSpec fbSpec;
namespace XENGINE {
    Platform::Input input;

    // Platform
    void Init(const char *title, int windowX, int windowY) {
        Platform::CreateWindow(title, windowX, windowY);
        fbSpec.width = windowX;
        fbSpec.height = windowY;
        renderer.Init(fbSpec);
        
        fb.Initialize(fbSpec);
    }
    
    bool WindowShouldClose() {
        return Platform::WindowShouldClose();
    }
    
    void StartDrawing(Shader::Shader* Shader) {
        // Platform::UpdateWindow(fb, fbSpec);
        // fb.Update(fbSpec);
        activeScene = &g_sceneManager.GetCurrentActiveScene();
        // std::cout << "[Engine] activeScene allocation: " << activeScene << "\n";
        
        fb.Bind();
        renderer.ClearScreen();
        renderer.StartDrawing(Shader, activeScene->GetActiveCamera(), *activeScene);
        fb.Unbind();
        activeScene->UpdateScene();
        std::cout << "[Engine] Scene ECS allocation: " << &activeScene->ecs << "\n";
        if (activeScene->ecs.HasComponent<XENGINE::TransformComponent>(0)) {
            std::cout << "[Engine] Cube has Transform component\n";
        } else {
            std::cout << "[Engine] Cube doesnt have Transform component\n";        
        }

        renderer.ClearScreen();
        renderer.DrawQuadMesh(fb.GetTextureID());

        input.Update();
    }
    
    void EndDrawing() {
        Platform::SwapBuffers();
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
            std::cout << "GL Error: " << err << "\n";
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
        activeScene = &g_sceneManager.GetCurrentActiveScene();
    }

    XENGINE::Scene* GetActiveScene() {
        return activeScene;
    }
} 