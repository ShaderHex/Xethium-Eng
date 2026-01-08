#include "texture/texture.h"
#include "stb_image.h"
#include "glad/glad.h"

#include <iostream>

namespace Texture {
    Texture::Texture(const char* path) {
        stbi_set_flip_vertically_on_load(true);

        m_Data = stbi_load(path, &m_Width, &m_Height, &m_NrChannels, 0);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }

    void Texture::Bind() {
        glGenTextures(1, &m_Texture);
        glBindTexture(GL_TEXTURE_2D, m_Texture);
        if (m_Data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Data);
            glGenerateMipmap(GL_TEXTURE_2D);       
        } else {
            std::cout<<"Failed to load the texture\n";
        }
        stbi_image_free(m_Data);
    }
}
