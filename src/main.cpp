#include "Core/Application.h"

int main() {
    Application app;

    if (!app.Init()) {
        return -1; // Failed to initialize
    }

    while (!WindowShouldClose()) {
        app.Render(); // Run the render loop
    }

    CloseWindow(); // Cleanup
    return 0;
}
