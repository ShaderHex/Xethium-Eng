#define GLFW_EXPOSE_NATIVE_WIN32
#include "XEngine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "input/keycode.h"
#include "texture/texture.h"
#include "resourceManager/resourceManager.h" 
#include "platform/platform.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

auto camera = XENGINE::CreateCamera();
float dt;
float fps;

void CreateInputKeys() {
    XENGINE::CreateAction("forward", X_KEY_W);
    XENGINE::CreateAction("backward", X_KEY_S);
    XENGINE::CreateAction("right", X_KEY_D);
    XENGINE::CreateAction("left", X_KEY_A);
    XENGINE::CreateAction("up", X_KEY_Q);
    XENGINE::CreateAction("down", X_KEY_E);
    XENGINE::CreateAction("change_texture", X_KEY_F);
    XENGINE::CreateAction("unload_texture", X_KEY_X);
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

void InitImGui() {
    std::cout << "[testbed] ImGui init started \n";
    IMGUI_CHECKVERSION();
    std::cout << "[testbed] ImGui ImGui version checked \n";
    ImGui::CreateContext();
    std::cout << "[testbed] ImGui created contect \n";
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    std::cout << "[testbed] ImGui ImGui assigned io \n";
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls 
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable gamepad controls
    std::cout << "[testbed] ImGui Keyboard and gamepad assigned \n";

    ImGui::StyleColorsDark();
    std::cout << "[testbed] ImGui style set \n";

    std::cout << "[testbed] Window address on initializaiton" << XENGINE::GetNativeWindow() << "\n";
    ImGui_ImplGlfw_InitForOpenGL(XENGINE::GetNativeWindow(), true);
    std::cout << "[testbed] ImGui init glfw for opengl finished\n";
    ImGui_ImplOpenGL3_Init("#version 330");
    std::cout << "[testbed] ImGui init opengl finished\n";
    
}

void ImGuiStartFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiEndFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ShutdownImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void InitTestbed() {
    XENGINE::Init("testbed", 1200, 800);
    CreateInputKeys();
    glfwInit();

    // Imgui initialization
    InitImGui();
}

void UpdateFPSTitle(GLFWwindow* window, float dt) {
    static float timer = 0.0f;
    static int frames = 0;

    timer += dt;
    frames++;

    if (timer >= 1.0f) {
        fps = frames / timer;
        
        std::string title = "testbed - FPS: " + std::to_string((int)fps);
        XENGINE::ChangeNativeWindowTitle(title.c_str());
        
        timer -= 1.0f; 
        frames = 0;
    }
}

int main() {
    InitTestbed();

    auto shader = XENGINE::CreateShader("shaders/vertex.vs", "shaders/fragment.fs");
    auto* cube1 = XENGINE::CreateCube(0, 1.5f, 0, 0, 0, 0, 1, 1, 1, {255, 255, 255});
    XENGINE::CreateCube(0, 0, 0, 0, 0, 0, 1, 1, 1, {255, 0, 0});
    XENGINE::CreateCube(1.5f, 0, 0, 0, 0, 0, 1, 1, 1, {255, 255, 0});

    std::shared_ptr<Texture::Texture> wall = XENGINE::ResourceManager::LoadTexture("wall.jpg");
    std::shared_ptr<Texture::Texture> tex = XENGINE::ResourceManager::LoadTexture("texture.png");


    cube1->texture = tex;

    bool wall_tex = true;

    float fpsData[] = {5.0, 3.0, 5.4, 3.7, 10};
    std::vector<float> fpsHistory = {0};
    int i = 0;
    
    while (!XENGINE::WindowShouldClose()) {
        dt = XENGINE::GetDeltaTime();
        UpdateFPSTitle(XENGINE::GetNativeWindow(), dt);
        
        fpsHistory.erase(fpsHistory.begin());
        if(*fpsHistory.data() >= 15.0) {
        }
        float currentFPS = 1.0f / dt;
        fpsHistory.push_back(currentFPS);

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
        
        ImGuiStartFrame();
        XENGINE::StartDrawing(shader, camera);


        //XENGINE::useShader(shader);

        ImGui::Begin("Debug");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Cubes created: %.i", i);
        ImGui::PlotLines("##fps", fpsHistory.data(), fpsHistory.size(), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 150));
        ImGui::End();
        
        ImGuiEndFrame();
        XENGINE::EndDrawing();
    }

    ShutdownImGui();
    XENGINE::CloseWindow();
}