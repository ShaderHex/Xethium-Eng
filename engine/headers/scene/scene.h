#pragma once
#include "../../include/defines.h"
#include "gameObject/gameObject.h"
#include "transform/transform.h"
#include "texture/texture.h"
#include "ecs/entityManager.h"
#include "camera/camera.h"
#include <vector>

namespace XENGINE {

class Scene {
public:
    XAPI Scene();
    XAPI void CreateCube(GameObject::GameObject::CubeSpec spec, Color color);
    XAPI void SetActiveCamera(Camera::Camera& activeCamera);
    XAPI Entity CreateEntity();
    XAPI void UpdateScene();

    GameObject::GameObject& GetGameObject() { return m_ObjectContainer; } 
    Camera::Camera& GetActiveCamera() { return m_activeCamera; } 

    XENGINE::EntityManager ecs;
    Camera::Camera m_activeCamera;
private:
    GameObject::GameObject m_ObjectContainer;
};

}