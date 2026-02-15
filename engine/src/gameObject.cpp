#include "transform/transform.h"
#include "mesh/mesh.h"
#include "mesh/meshFactory.h"
#include "gameObject/gameObject.h"
#include "glad/glad.h"

namespace GameObject {
void GameObject::CreateCube(float x, float y, float z) {
    cubeMesh = MeshFactory::MeshFactory::CreateCube();
    
    cubeObject.mesh = cubeMesh;

    cubeObject.transform.position = {x, y, z};
    cubeObject.transform.rotation = {0.0f, 0.0f, 0.0f};
    cubeObject.transform.scale = {1.0f, 1.0f, 1.0f};

    m_cubeObjects.push_back(cubeObject);
} 

void GameObject::Render(Shader::Shader* shader) {
    for (auto& obj : m_cubeObjects) {
        if (obj.mesh) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, obj.transform.position);
            float angle = 20.0f;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.1f, 0.3f, 0.5f));
            model = glm::scale(model, glm::vec3(5.0f));
            shader->setMat4("model", model);

            glBindVertexArray(obj.mesh->VAO);
            glDrawArrays(GL_TRIANGLES, 0, obj.mesh->vertexCount);
            glBindVertexArray(0);

        }
    }
}

}
