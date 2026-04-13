#pragma once
#include "../../include/defines.h"
#include "texture/texture.h"
#include "glad/glad.h"
#include <memory>

namespace XENGINE {

namespace ResourceManager {

XAPI std::shared_ptr<Texture::Texture> LoadTexture(const char* texturePath);
XAPI std::shared_ptr<Texture::Texture> CreateBlankTexture(int width, int height);
XAPI bool UnoadTexture(std::shared_ptr<Texture::Texture>* texture);
XAPI void Test();

}

}