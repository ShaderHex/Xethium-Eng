#include "transform/transform.h"
#include "mesh/mesh.h"
#include "mesh/meshFactory.h"
#include "gameObject/gameObject.h"
#include "glad/glad.h"

namespace GameObject {

void GameObject::CreateCube(float x, float y, float z, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ, Color color) {
    cubeMesh = MeshFactory::MeshFactory::CreateCube();
    
    GameObject::Object cubeObject;

    cubeObject.mesh = cubeMesh;

    cubeObject.transform.position = {x, y, z};
    cubeObject.transform.rotation = {rotX, rotY, rotZ};
    cubeObject.transform.scale = {scaleX, scaleY, scaleZ};
    cubeObject.transform.color = {color};

    m_cubeObjects.push_back(cubeObject);
} 

void GameObject::Render(Shader::Shader* shader) {
    for (auto& obj : m_cubeObjects) {
        if (obj.mesh) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, obj.transform.position);
            
            model = glm::rotate(model, glm::radians(obj.transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(obj.transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(obj.transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

            model = glm::scale(model, glm::vec3(obj.transform.scale.x, obj.transform.scale.y, obj.transform.scale.z));
            shader->use();
            shader->setVec3("objectColor", {obj.transform.color});
            
            shader->setMat4("model", model);

            glBindVertexArray(obj.mesh->VAO);
            glDrawArrays(GL_TRIANGLES, 0, obj.mesh->vertexCount);
            glBindVertexArray(0);

        }
    }
}

}
