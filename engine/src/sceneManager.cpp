#include "scene/sceneManager.h"
#include <iostream>

namespace XENGINE {

void sceneManager::SwitchActiveScene(XENGINE::Scene& scene) {
    m_activeScene = &scene;
    std::cout << "[SceneManager] Switched to scene on address at: " << &m_activeScene << "\n";
}

}