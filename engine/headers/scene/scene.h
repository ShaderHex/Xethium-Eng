#pragma once
#include "../../include/defines.h"
#include "gameObject/gameObject.h"
#include "transform/transform.h"
#include <vector>

namespace XENGINE {

namespace SceneData {

struct CubeSpec {
    Transform::Vector3 position;
    Transform::Vector3 rotation;
    Transform::Vector3 scale;
};

}

class Scene {
public:
    XAPI GameObject::GameObject::Object* CreateCube(SceneData::CubeSpec spec, Color color);

    const std::vector<GameObject::GameObject>& GetGameObject() const { return m_gameObject; } 

private:
    std::vector<GameObject::GameObject> m_gameObject;
};

}