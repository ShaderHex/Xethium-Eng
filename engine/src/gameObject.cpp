#include "transform/transform.h"
#include "mesh/mesh.h"
#include "mesh/meshFactory.h"
#include "gameObject/gameObject.h"
#include "glad/glad.h"
#include <iostream>

namespace GameObject {

GameObject::Object* GameObject::CreateCube(CubeSpec CubeSpec, Color color) {
    std::cout<< "Creating Cube\n";
    m_cubeObjects.emplace_back();
    cubeMesh = MeshFactory::MeshFactory::CreateCube();
    // if (texturePath != nullptr) {
    //     m_texture = new Texture::Texture(texturePath);
    // }
    
    GameObject::Object cubeObject;

    cubeObject.mesh = cubeMesh;

    cubeObject.transform.position = {CubeSpec.position.x, CubeSpec.position.y, CubeSpec.position.z};
    cubeObject.transform.rotation = {CubeSpec.rotation.x, CubeSpec.rotation.y, CubeSpec.rotation.z};
    cubeObject.transform.scale = {CubeSpec.scale.x, CubeSpec.scale.y, CubeSpec.scale.z};
    cubeObject.transform.color = {color};
    cubeObject.texture = CubeSpec.texture;
        
    m_cubeObjects.push_back(cubeObject);
    std::cout<< "Cube created\n";

    return &m_cubeObjects.back();
} 

// void GameObject::setTexture() {

// }

void GameObject::Render(Shader::Shader* shader) {
    shader->use();

    for (auto& obj : m_cubeObjects) {
        if (obj.mesh) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, obj.transform.position);
            
            model = glm::rotate(model, glm::radians(obj.transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(obj.transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(obj.transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

            model = glm::scale(model, glm::vec3(obj.transform.scale.x, obj.transform.scale.y, obj.transform.scale.z));
            
            // if (obj.texture = NULL) {
            //     std::cout << "obj texture is null\n";
                
            // } else {
            //     std::cout << "obj texture is not null\n";
            //     std::cout << "finished binding\n";
            // }

      
            // std::cout << obj.texture << "\n";

            
            
            if (!obj.texture) {
                shader->setBool("useTexture", false);
                // std::cout << "Cube has no texture!\n";
            } else {
                shader->setBool("useTexture", true);
                obj.texture->Bind();
            }
            shader->setVec3("objectColor", {obj.transform.color});
            //shader->setInt("texture1", 1);
            shader->setMat4("model", model);

            glBindVertexArray(obj.mesh->VAO);
            glDrawArrays(GL_TRIANGLES, 0, obj.mesh->vertexCount);
            glBindVertexArray(0);

        }
    }
}

}
