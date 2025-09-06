// To use the skybox (and other assets), make sure the "project" folder is placed 
// in the same directory as the engine executable. 
// Example: 
//  xethium/               <- engine executable here
//      project/           <- copy your project folder here
//          assets/skybox/ <- skybox textures go here
//
// This ensures the engine can load all required assets correctly.
// In the future, a project creator/launcher will handle this automatically.


#include "Core/Application.h"
#include "Renderer/Renderer.h"

int main() {
    Application app;
    app.Init();
    app.Run();
    app.Shutdown();
    return 0;
}
