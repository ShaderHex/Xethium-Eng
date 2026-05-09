#include "scene/scene.h"
#include "ecs/componentStorage.h"
#include "mesh/meshFactory.h"
#include <iostream>

namespace XENGINE{

Scene::Scene() {
    std::cout << "[Scene] New Scene created at " << this << "\n";
}

GameObject::GameObject::Object& Scene::CreateCube(GameObject::GameObject::CubeSpec spec, Color color) {
    static auto* cubeMesh = MeshFactory::MeshFactory::CreateCube();
    // GameObject::GameObject& gameObject = m_gameObject.back();
    XENGINE::TransformComponent transform;
    XENGINE::MeshComponent mesh;

    Entity Entity = ecs.CreateEntity();

    mesh.Mesh = cubeMesh;
    
    ecs.AddComponent<XENGINE::TransformComponent>(Entity, transform);
    ecs.AddComponent<XENGINE::MeshComponent>(Entity, mesh);

    return m_ObjectContainer.m_cubeObjects.back();
}

}