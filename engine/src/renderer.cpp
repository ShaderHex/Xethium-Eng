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
#include "framebuffer/framebuffer.h"
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


void Renderer::Init() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
    }
    std::cout << "CREATED VBO, VAO, EBO VARIABLES" <<std::endl;

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);
    

}

void Renderer::processInput() {
    // input.Update();
    // if (input.IsActionHeld("bye")) {
    //     std::cout << "bye is held\n";
    // }
}

void Renderer::Draw(XENGINE::Scene activeScene, Shader::Shader* Shader) {

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
            
            if (activeScene.ecs.HasComponent<XENGINE::MeshComponent>(e)) {
                auto& mesh = activeScene.ecs.GetComponent<XENGINE::MeshComponent>(e);
                if(mesh.Mesh != nullptr) {
                    // std::cout << "[Renderer] obj mesh VAO ID: " << obj.mesh->VAO << " [Renderer] obj mesh vertexCount:" << obj.mesh->vertexCount << "\n";
                    glBindVertexArray(mesh.Mesh->VAO);
                    glDrawArrays(GL_TRIANGLES, 0, mesh.Mesh->vertexCount);
                    glBindVertexArray(0);
                } else {
                    std::cout << "[Renderer] obj mesh is null!\n";
                }
            }
        }
    }

}

void Renderer::StartDrawing(Shader::Shader* Shader, Camera::Camera camera, XENGINE::Scene activeScene) {
    glfwPollEvents();

    Shader->use();
    
    glm::mat4 view;
    view = glm::lookAt(camera.position, camera.front + camera.position, camera.up);
    camera.UpdateVectors();
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
    glClearColor(0.0, 0.0, 0.0, 1);
}

}
