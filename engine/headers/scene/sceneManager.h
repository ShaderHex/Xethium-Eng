#pragma once
#include "scene/scene.h"

namespace XENGINE {

class sceneManager {
public:
    void SwitchActiveScene(XENGINE::Scene& scene);
    XENGINE::Scene& GetCurrentActiveScene() { return *m_activeScene; }
private:
    XENGINE::Scene* m_activeScene;
};

}