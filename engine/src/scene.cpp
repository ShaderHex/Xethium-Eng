#include "scene/scene.h"

namespace XENGINE{

GameObject::GameObject::Object* Scene::CreateCube(SceneData::CubeSpec spec, Color color) {
    m_gameObject.emplace_back();

    GameObject::GameObject& gameObject = m_gameObject.back();

    gameObject.CreateCube(spec.position.x, spec.position.y, spec.position.z, spec.rotation.x, spec.rotation.y, spec.rotation.z, spec.scale.x, spec.scale.y, spec.scale.z, color);

    return &gameObject.m_cubeObjects.back();
}

}