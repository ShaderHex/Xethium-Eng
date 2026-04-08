#include "XEngine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "input/keycode.h"
#include "texture/texture.h"
#include "resourceManager/resourceManager.h" 

#include <iostream>

auto camera = XENGINE::CreateCamera();
float dt;

void CreateInputKeys() {
    XENGINE::CreateAction("forward", X_KEY_W);
    XENGINE::CreateAction("backward", X_KEY_S);
    XENGINE::CreateAction("right", X_KEY_D);
    XENGINE::CreateAction("left", X_KEY_A);
    XENGINE::CreateAction("up", X_KEY_Q);
    XENGINE::CreateAction("down", X_KEY_E);
    XENGINE::CreateAction("change_texture", X_KEY_F);
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

    if(XENGINE::IsActionHeld("up")) {
        camera.position.y += camSpeed * dt;
    }

    if(XENGINE::IsActionHeld("down")) {
        camera.position.y -= camSpeed * dt;
    }
}



int main() {
    XENGINE::Init("testbed", 1200, 800);

    auto shader = XENGINE::CreateShader("shaders/vertex.vs", "shaders/fragment.fs");
    auto* cube1 = XENGINE::CreateCube(0, 1.5f, 0, 0, 0, 0, 1, 1, 1, {255, 255, 255});
    auto* cube2 = XENGINE::CreateCube(0, 0, 0, 0, 0, 0, 1, 1, 1, {255, 0, 0});
    XENGINE::CreateCube(1.5f, 0, 0, 0, 0, 0, 1, 1, 1, {255, 255, 0});

    Texture::Texture* wall = XENGINE::ResourceManager::LoadTexture("wall.jpg");
    Texture::Texture* tex = XENGINE::ResourceManager::LoadTexture("texture.png");

    CreateInputKeys();

    cube1->texture = wall;

    float test = 0;
    bool wall_tex = true;

    while (!XENGINE::WindowShouldClose()) {
        dt = XENGINE::GetDeltaTime();

        if (XENGINE::IsActionPressed("change_texture")) {
            wall_tex = !wall_tex;
        }

        cube1->transform.position.x += 0.1 * dt;

        if (wall_tex == true) {
            cube1->texture = wall;
        } else {
            cube1->texture = tex;
        }


        UpdateInput();
        // XENGINE::ResourceManager::Test();
        
        XENGINE::StartDrawing(shader, camera);


        //XENGINE::useShader(shader);
        
        XENGINE::EndDrawing();
    }
    XENGINE::CloseWindow();
}
