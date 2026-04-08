#pragma once
#include "../../include/defines.h"
#include "texture/texture.h"

namespace XENGINE {

namespace ResourceManager {
XAPI Texture::Texture* LoadTexture(const char* texturePath);
XAPI void Test();

}


}