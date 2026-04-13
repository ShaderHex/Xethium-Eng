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

std::shared_ptr<Texture::Texture> CreateBlankTexture(int width, int height) {
    std::shared_ptr<Texture::Texture> texture = std::make_shared<Texture::Texture>(nullptr);
    
    if (!texture) {
        std::cout << "[ResourceManager] Failed to load custom texture with texture ID: " << texture.get() << "\n"; 
    }

    texture->CreateTexture(width, height);
    texture->Bind();

    std::cout << "[ResourceManager] Loaded custom texture with texture ID: " << texture.get() << "\n";
    std::cout << "[ResourceManager] texture: " << texture.get() << " is used " << texture.use_count() << " times\n";

    return texture;

}

void Test() {
    // std::cout<<"Test";
}

}

}