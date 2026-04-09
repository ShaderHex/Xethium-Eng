#include "resourceManager/resourceManager.h"
#include "stb_image.h"
#include <iostream>

namespace XENGINE {

namespace ResourceManager {

    
std::shared_ptr<Texture::Texture> LoadTexture(const char* texturePath) {
    std::shared_ptr<Texture::Texture> texture = std::make_shared<Texture::Texture>(texturePath);
    if (!texture) {
        std::cout << "[ResourceManager] Failed to load " << texturePath << " texture ID: " << texture.get() << "\n"; 
    }
    std::cout << "[ResourceManager] Loaded " << texturePath << " with texture ID: " << texture.get() << "\n";
    std::cout << "[ResourceManager] texture: " << texture.get() << " is used " << texture.use_count() << " times\n";
    return texture;
}

void Test() {
    // std::cout<<"Test";
}

}

}