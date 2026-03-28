#include "XEngine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "input/keycode.h"

#include <iostream>

auto camera = XENGINE::CreateCamera();
float dt;

void CreateInputKeys() {
    XENGINE::CreateAction("forward", X_KEY_W);
    XENGINE::CreateAction("backward", X_KEY_S);
    XENGINE::CreateAction("right", X_KEY_D);
    XENGINE::CreateAction("left", X_KEY_A);
}

void UpdateInput() {
    float camSpeed = 5.0f;

    if(XENGINE::IsActionHeld("forward")) {
        camera.position.z -= camSpeed * dt;
    }

    if(XENGINE::IsActionHeld("backward")) {
        camera.position.z += camSpeed * dt;
    }

    if(XENGINE::IsActionHeld("right")) {
        camera.position.x += camSpeed * dt;
    }

    if(XENGINE::IsActionHeld("left")) {
        camera.position.x -= camSpeed * dt;
    }
}

int main() {
    XENGINE::Init("testbed", 1200, 800);

    auto shader = XENGINE::CreateShader("shaders/vertex.vs", "shaders/fragment.fs");
    XENGINE::CreateCube(0, 0, 0, 0, 0, 0, 1, 1, 1, {255, 0, 0});
    XENGINE::CreateCube(0, 1, 0, 0, 0, 0, 10, 1, 1, {255, 255, 255});

    CreateInputKeys();


    while (!XENGINE::WindowShouldClose()) {
        dt = XENGINE::GetDeltaTime();

        UpdateInput();
        
        XENGINE::StartDrawing(shader, camera);


        //XENGINE::useShader(shader);
        
        XENGINE::EndDrawing();
    }
    XENGINE::CloseWindow();
}
