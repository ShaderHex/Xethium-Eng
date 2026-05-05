#pragma once
#include "../../include/defines.h"
#include "gameObject/gameObject.h"
#include "transform/transform.h"
#include "texture/texture.h"
#include "EntityManager/EntityManager.h"
#include <vector>

namespace XENGINE {

class Scene {
public:
    XAPI Scene();
    XAPI GameObject::GameObject::Object& CreateCube(GameObject::GameObject::CubeSpec spec, Color color);

    GameObject::GameObject& GetGameObject() { return m_ObjectContainer; } 

    XENGINE::EntityManager ecs;
private:
    GameObject::GameObject m_ObjectContainer;
};

}