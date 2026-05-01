#include "scene/scene.h"
#include <iostream>

namespace XENGINE{

Scene::Scene() {
    std::cout << "[Scene] New Scene created at " << this << "\n";
}

GameObject::GameObject::Object& Scene::CreateCube(GameObject::GameObject::CubeSpec spec, Color color) {
    // GameObject::GameObject& gameObject = m_gameObject.back();
    
    m_ObjectContainer.CreateCube(spec, color);

    return m_ObjectContainer.m_cubeObjects.back();
}

}