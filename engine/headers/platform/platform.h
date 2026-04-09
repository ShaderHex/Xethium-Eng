#pragma once

struct GLFWwindow;

namespace Platform {
    void CreateWindow(const char* title, int width, int height);
    bool WindowShouldClose();
    void PollEvents();
    void SwapBuffers();
    GLFWwindow* GetNativeWindow();
    void ChangeNativeWindowTitle(const char* title);
    float GetDeltaTime();
    void Shutdown();
}