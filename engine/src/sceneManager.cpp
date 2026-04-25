#include "scene/sceneManager.h"

namespace XENGINE {

void sceneManager::SwitchActiveScene(XENGINE::Scene scene) {
    m_activeScene = scene;
}

}