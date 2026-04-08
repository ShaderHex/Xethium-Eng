#include "resourceManager/resourceManager.h"
#include <iostream>

namespace XENGINE {

namespace ResourceManager {
Texture::Texture* texture = nullptr;

Texture::Texture* LoadTexture(const char* texturePath) {
    if (texture != nullptr) {
        delete texture;
    }
    texture = new Texture::Texture(texturePath);
    
    if (!texture) {
        std::cout << "[ResourceManager] Failed to load " << texturePath << " texture ID: " << texture << "\n"; 
    }
    std::cout << "[ResourceManager] Loaded " << texturePath << " with texture ID: " << texture << "\n"; 
    
    return texture;
}

void Test() {
    // std::cout<<"Test";
}

}

}