#include "Core/Application.h"

int main() {
    Application app;

    if (!app.Init()) {
        return -1; 
    }

    while (!WindowShouldClose()) {
        app.Render(); 
    }

    app.CleanUp();
    return 0;
}
