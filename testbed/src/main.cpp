#define GLFW_EXPOSE_NATIVE_WIN32
#include "XEngine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "input/keycode.h"
#include "texture/texture.h"
#include "resourceManager/resourceManager.h" 
#include "platform/platform.h"
#include "scene/scene.h"
#include "scene/sceneManager.h"
#include "ecs/entityManager.h"
#include "ecs/componentStorage.h"
#include "mesh/meshFactory.h"
#include "mesh/mesh.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

auto camera = XENGINE::CreateCamera();
float dt;
float fps;
bool is_first_scene;

void CreateInputKeys() {
    XENGINE::CreateAction("forward", X_KEY_W);
    XENGINE::CreateAction("backward", X_KEY_S);
    XENGINE::CreateAction("right", X_KEY_D);
    XENGINE::CreateAction("left", X_KEY_A);
    XENGINE::CreateAction("up", X_KEY_Q);
    XENGINE::CreateAction("down", X_KEY_E);
    XENGINE::CreateAction("change_texture", X_KEY_F);
    XENGINE::CreateAction("unload_texture", X_KEY_X);
    XENGINE::CreateAction("change_scene", X_KEY_C);
}

void UpdateInput(XENGINE::Scene& scene, XENGINE::Scene& scene2) {
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

    if (XENGINE::IsActionPressed("change_scene")) {
        if (is_first_scene == true) {
            XENGINE::SwitchActiveScene(scene);
            is_first_scene = false;
        } else {
            std::cout<<"Switching scene to " << &scene2 << "\n";
            XENGINE::SwitchActiveScene(scene2);
            is_first_scene = true;
        }
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
        
        std::string title = "Testbed - FPS: " + std::to_string((int)fps);
        XENGINE::ChangeNativeWindowTitle(title.c_str());
        
        timer -= 1.0f; 
        frames = 0;
    }
}

int main() {
    InitTestbed();

    auto shader = XENGINE::CreateShader("shaders/vertex.vs", "shaders/fragment.fs");
    // auto* cube1 = XENGINE::CreateCube(0, 1.5f, 0, 0, 0, 0, 1, 1, 1, {255, 255, 255});
    // XENGINE::CreateCube(0, 0, 0, 0, 0, 0, 1, 1, 1, {255, 0, 0});
    // XENGINE::CreateCube(1.5f, 0, 0, 0, 0, 0, 1, 1, 1, {255, 255, 0});

    std::shared_ptr<Texture::Texture> wall = XENGINE::ResourceManager::LoadTexture("wall.jpg");
    std::shared_ptr<Texture::Texture> tex = XENGINE::ResourceManager::LoadTexture("texture.png");
    std::shared_ptr<Texture::Texture> customTex = XENGINE::ResourceManager::CreateBlankTexture(512, 512);


    // cube1->texture = customTex;

    bool wall_tex = true;
    is_first_scene = true;

    std::vector<float> fpsHistory = {0};
    int i = 0;
    
    int fpsPlotLineValue = 100;

    // First scene
    XENGINE::Scene scene;
    GameObject::GameObject::CubeSpec Cube1;
    // Cube.position.x = 0.0f;
    // Cube.position.y = 1.5f;
    // Cube.position.z = 0.0f;

    // Cube.scale.x = 1.0f;
    // Cube.scale.y = 1.0f;
    // Cube.scale.z = 1.0f;

    // Cube.rotation.x = 0.0f;
    // Cube.rotation.y = 0.0f;
    // Cube.rotation.z = 0.0f;

    // // Cube.texture = tex;

    // scene.CreateCube(Cube, {255, 255, 255});

    // // Second Scene
    XENGINE::Scene scene2;
    // GameObject::GameObject::CubeSpec Cube2;
    // Cube2.texture = tex;
    // scene2.CreateCube(Cube2, {255, 255, 255});
    // scene2.CreateCube(Cube, {255, 255, 255});
    XENGINE::SwitchActiveScene(scene); // starting the engine with "scene" scene
    Entity Cube = scene.ecs.CreateEntity();
    
    // the cube mesh
    static Mesh::Mesh* cubeMesh = MeshFactory::MeshFactory::CreateCube();
    XENGINE::MeshComponent mesh;
    mesh.Mesh = cubeMesh;
    
    // Transform component (position, scale and rotation)
    XENGINE::TransformComponent cubeTransform;
    
    scene.ecs.AddComponent<XENGINE::TransformComponent>(Cube, cubeTransform);
    scene.ecs.AddComponent<XENGINE::MeshComponent>(Cube, mesh);
    
    std::cout << "[Testbed] Scene ECS allocation: " << &scene.ecs << "\n";
    
    if (scene.ecs.HasComponent<XENGINE::TransformComponent>(Cube)) {
        std::cout << "[Testbed] Cube has Transform component\n";
    } else {
        std::cout << "[Testbed] Cube doesnt have Transform component\n";        
    }
    
    if (scene.ecs.HasComponent<XENGINE::MeshComponent>(Cube)) {
        std::cout << "[Testbed] Cube has Mesh component\n";
    } else {
        std::cout << "[Testbed] Cube doesnt have Mesh component\n";
    }
    
    // scene.CreateCube(Cube1, {1,1,1});
    std::cout << "[Testbed] activeScene allocation: " << &scene << "\n";
    std::cout << "[Testbed] Entities count: " << scene.ecs.GetEntityCount() << "\n";
    
    // scene2.Update(function)
    while (!XENGINE::WindowShouldClose()) {
        std::cout << "[Testbed] Transform type hash: "
              << typeid(XENGINE::TransformComponent).name()
              << "\n";
        
        scene.SetActiveCamera(camera);
        dt = XENGINE::GetDeltaTime();
        UpdateFPSTitle(XENGINE::GetNativeWindow(), dt);
        
        
        if(fpsHistory.size() >= 100.0) {
            fpsHistory.erase(fpsHistory.begin());
        }
        float currentFPS = 1.0f / dt;
        fpsHistory.push_back(currentFPS);
        
        if (XENGINE::IsActionPressed("change_texture")) {
            wall_tex = !wall_tex;
        }
        
        // cube1->transform.position.x += 0.1 * dt;
        
        // if (wall_tex == true) {
        //     cube1->texture = customTex;
        // } else {
        //     cube1->texture = tex;
        // }


        UpdateInput(scene, scene2);
        
        ImGuiStartFrame();
        XENGINE::StartDrawing(shader);


        //XENGINE::useShader(shader);

        ImGui::Begin("Debug");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Cubes created: %.i", i);
        ImGui::PlotLines("##fps", fpsHistory.data(), fpsPlotLineValue, 0, nullptr, 0.0f, 5000.0f, ImVec2(0, 150));
        ImGui::Text("Camera X: %f", scene.GetActiveCamera().position.x);
        ImGui::Text("Camera Y: %f", scene.GetActiveCamera().position.y);
        ImGui::Text("Camera Z: %f", scene.GetActiveCamera().position.z);
        ImGui::DragInt("##plot", &fpsPlotLineValue, 0.5);
        ImGui::End();
        
        ImGuiEndFrame();
        XENGINE::EndDrawing();
    }

    ShutdownImGui();
    XENGINE::CloseWindow();
}