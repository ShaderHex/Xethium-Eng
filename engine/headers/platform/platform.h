#include "GLFW/glfw3.h"

namespace Platform {
    void CreateWindow(const char* title, int width, int height);
    bool WindowShouldClose();
    void PollEvents();
    void SwapBuffers();
    GLFWwindow* GetNativeWindow();
    void Shutdown();
}
