#include "scene/scene.h"
#include "ecs/componentStorage.h"
#include "mesh/meshFactory.h"
#include <iostream>

namespace XENGINE{

Scene::Scene() {
    std::cout << "[Scene] New Scene created at " << this << "\n";
}

void Scene::SetActiveCamera(Camera::Camera& activeCamera) {
    // std::cout << "[Scene] Camera X: " << activeCamera.position.x << " Y: " << activeCamera.position.y << "\n";
    // std::cout << "[Scene] Scene allocation: " << this << "\n";
    m_activeCamera = activeCamera;
}

// These are preset for creating different type of entities!

void Scene::CreateCube(GameObject::GameObject::CubeSpec spec, Color color) {
    static Mesh::Mesh* cubeMesh = MeshFactory::MeshFactory::CreateCube();
    XENGINE::TransformComponent transform;
    XENGINE::MeshComponent mesh;

    Entity Entity = ecs.CreateEntity();

    mesh.Mesh = cubeMesh;
    
    ecs.AddComponent<XENGINE::TransformComponent>(Entity, transform);
    ecs.AddComponent<XENGINE::MeshComponent>(Entity, mesh);

    std::cout << "[Scene] Entities count: " << ecs.GetEntityCount() << "\n";
}

Entity Scene::CreateEntity() {
    std::cout << "[Scene] Scene ECS allocation: " << &ecs << "\n";
    return ecs.CreateEntity();
}

void Scene::UpdateScene() {
    m_activeCamera.UpdateVectors();
}

}
