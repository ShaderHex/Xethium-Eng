#include "Core/Application.h"
#include "Renderer/Renderer.h"

int main() {
    Application app;
    app.Init();
    app.Run();
    app.Shutdown();
    return 0;
}
