#pragma once
#include "../../include/defines.h"
#include "gameObject/gameObject.h"
#include "transform/transform.h"
#include "texture/texture.h"
#include <vector>

namespace XENGINE {

namespace SceneData {

struct CubeSpec {
    Transform::Vector3 position;
    Transform::Vector3 rotation;
    Transform::Vector3 scale;

    Texture::Texture texture;
};

}

class Scene {
public:
    XAPI Scene();
    XAPI GameObject::GameObject::Object& CreateCube(GameObject::GameObject::CubeSpec spec, Color color);

    const GameObject::GameObject& GetGameObject() const { return m_ObjectContainer; } 

private:
    GameObject::GameObject m_ObjectContainer;
};

}