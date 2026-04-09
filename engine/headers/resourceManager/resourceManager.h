#pragma once
#include "../../include/defines.h"
#include "texture/texture.h"
#include "glad/glad.h"
#include <memory>

namespace XENGINE {

namespace ResourceManager {

XAPI std::shared_ptr<Texture::Texture> LoadTexture(const char* texturePath);
XAPI bool UnoadTexture(std::shared_ptr<Texture::Texture>* texture);
XAPI void Test();

}

}