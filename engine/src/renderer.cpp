#include "renderer/renderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb_image.h"
#include "platform/platform.h"
#include "mesh/meshFactory.h"
#include "gameObject/gameObject.h"
#include "input/keycode.h"
#include "ECS/componentStorage.h"
#include "mesh/meshFactory.h"
#include <iostream>
#include <fstream>

namespace Renderer{

float texCoords[] = {
    0.0f, 0.0f,   // bottom-left
    1.0f, 0.0f,   // bottom-right
    0.5f, 1.0f    // top-center
};

// TODO: Store framebuffer spec in renderer
void Renderer::Init(XENGINE::framebuffer::FramebufferSpec fbSpec) {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
    }
    std::cout << "CREATED VBO, VAO, EBO VARIABLES" <<std::endl;

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);
    
    m_textureColorbuffer = XENGINE::ResourceManager::CreateBlankTexture(fbSpec.width, fbSpec.height);
    m_framebufferShader = std::make_unique<Shader::Shader>("shaders/framebuffer.vs", "shaders/framebuffer.fs");
    rabbit = std::make_unique<XENGINE::Model>("bunny.obj");

}

void Renderer::DrawQuadMesh(GLuint textureID) {
    static Mesh::Mesh* quad = MeshFactory::MeshFactory::CreateQuadMesh();

    m_framebufferShader->use();
    glBindVertexArray(quad->VAO);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    m_framebufferShader->setInt("screenTexture", 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::Draw(XENGINE::Scene& activeScene, Shader::Shader* Shader) {

    // for (auto& obj : gameObject.GetCubeObjects()) {
    //     glm::mat4 model = glm::mat4(1.0f);
    //         model = glm::translate(model, );
            
    //         model = glm::rotate(model, glm::radians(obj.transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    //         model = glm::rotate(model, glm::radians(obj.transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    //         model = glm::rotate(model, glm::radians(obj.transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    //         model = glm::scale(model, glm::vec3(obj.transform.scale.x, obj.transform.scale.y, obj.transform.scale.z));
            
    //         // if (obj.texture = NULL) {
    //         //     std::cout << "obj texture is null\n";
                
    //         // } else {
    //         //     std::cout << "obj texture is not null\n";
    //         //     std::cout << "finished binding\n";
    //         // }

      
    //         // std::cout << obj.texture << "\n";

            
            
    //         if (!obj.texture) {
    //             Shader->setBool("useTexture", false);
    //             // std::cout << "Cube has no texture!\n";
    //         } else {
    //             Shader->setBool("useTexture", true);
    //             obj.texture->Bind();
    //         }
    //         Shader->setVec3("objectColor", {obj.transform.color});
    //         //shader->setInt("texture1", 1);
    //         Shader->setMat4("model", model);

    //         if(obj.mesh != nullptr) {
    //             // std::cout << "[Renderer] obj mesh VAO ID: " << obj.mesh->VAO << " [Renderer] obj mesh vertexCount:" << obj.mesh->vertexCount << "\n";
    //             glBindVertexArray(obj.mesh->VAO);
    //             glDrawArrays(GL_TRIANGLES, 0, obj.mesh->vertexCount);
    //             glBindVertexArray(0);
    //         } else {
    //             std::cout << "[Renderer] obj mesh is null!\n";
    //         }
    //     }

    for (auto& e : activeScene.ecs.GetAllEntity()) {
        if (activeScene.ecs.HasComponent<XENGINE::TransformComponent>(e)) {
            auto& transform = activeScene.ecs.GetComponent<XENGINE::TransformComponent>(e);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, transform.position);
            
            model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

            model = glm::scale(model, glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));

            Shader->setBool("useTexture", false); // TODO: Texture after material

            Shader->setVec3("objectColor", {1, 0, 1}); // TODO: Color after material
            Shader->setMat4("model", model);
            // std::cout << "Entity with transform FOUND! rendering it...\n";
            
        }

        if (activeScene.ecs.HasComponent<XENGINE::MeshComponent>(e)) {
            auto& mesh = activeScene.ecs.GetComponent<XENGINE::MeshComponent>(e);
            if(mesh.Mesh != nullptr) {
                // std::cout << "[Renderer] obj mesh VAO ID: " << obj.mesh->VAO << " [Renderer] obj mesh vertexCount:" << obj.mesh->vertexCount << "\n";
                glBindVertexArray(mesh.Mesh->VAO);
                glDrawArrays(GL_TRIANGLES, 0, mesh.Mesh->vertexCount);
                glBindVertexArray(0);
                // std::cout << "Entity with mesh FOUND! rendering it...\n";
            } else {
                std::cout << "[Renderer] obj mesh is null!\n";
            }
        }
    }
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view  = activeScene.GetActiveCamera().GetViewMatrix();
    glm::mat4 proj  = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);

    // print to confirm they're not identity/garbage
    std::cout << "view[3]: " << view[3][0] << " " << view[3][1] << " " << view[3][2] << "\n";

    Shader->setMat4("model", model);
    Shader->setMat4("view", view);
    Shader->setMat4("projection", proj);

    rabbit->Draw(*Shader);

}

void Renderer::StartDrawing(Shader::Shader* Shader, Camera::Camera& camera, XENGINE::Scene& activeScene) {
    glfwPollEvents();

    Shader->use();
    
    glm::mat4 view;
    view = glm::lookAt(camera.position, camera.front + camera.position, camera.up);
    // camera.UpdateVectors();
    //Shader.setMat4();
    
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(camera.fov), 1200.0f / 800.0f, camera.nearPlane, camera.farPlane);
    
    int viewLoc = glGetUniformLocation(Shader->ID, "view");
    int projectionLoc = glGetUniformLocation(Shader->ID, "projection");
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    //glUniform1i(glGetUniformLocation(Shader->ID, "texture1"), 0);
    
    // gameObject.Render(Shader);
    this->Draw(activeScene, Shader);
    
    
    //Shader->setInt("texture1", 1);
    //m_texture->Bind();
}

void Renderer::ClearScreen() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1);
}

}
