#include "scene/scene.h"

namespace XENGINE{

Scene::Scene() {
    // m_gameObject.emplace_back();
}

GameObject::GameObject::Object& Scene::CreateCube(GameObject::GameObject::CubeSpec spec, Color color) {
    // GameObject::GameObject& gameObject = m_gameObject.back();

    m_ObjectContainer.CreateCube(spec, color);

    return m_ObjectContainer.m_cubeObjects.back();
}

}