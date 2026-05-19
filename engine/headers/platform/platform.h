#pragma once
#include "framebuffer/framebuffer.h"

struct GLFWwindow;

namespace Platform {
    void CreateWindow(const char* title, int width, int height);
    bool WindowShouldClose();
    void PollEvents();
    void SwapBuffers();
    GLFWwindow* GetNativeWindow();
    void ChangeNativeWindowTitle(const char* title);
    float GetDeltaTime();
    void UpdateWindow(XENGINE::framebuffer::Framebuffer& fb, XENGINE::framebuffer::FramebufferSpec& fbSpec);
    void Shutdown();
}