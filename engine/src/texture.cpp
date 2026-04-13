#include "texture/texture.h"
#include "stb_image.h"
#include "glad/glad.h"
#include <iostream>
#include <vector>

namespace Texture {

    Texture::Texture(const char* path) {
        stbi_set_flip_vertically_on_load(true);

        if (path == nullptr) {
            m_Data = nullptr;
            m_Width = 0;
            m_Height = 0;
            m_NrChannels = 0;
        } else {
            m_Data = stbi_load(path, &m_Width, &m_Height, &m_NrChannels, 0);
        }
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenTextures(1, &m_Texture);

    }

    void Texture::Bind() {
        // std::cout << "Started binding\n";
        glBindTexture(GL_TEXTURE_2D, m_Texture);
        // std::cout<<"Checking if image data is valid\n";
        if (m_Data){
            // std::cout<<"Image data is valid\n";
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Data);
            glGenerateMipmap(GL_TEXTURE_2D);       
            stbi_image_free(m_Data);
            m_Data = nullptr;
            std::cout << "[Texture] Uploaded image texture\n";
        }
        
        // std::cout<<"Unloaded the image data\n";
    }

    void Texture::CreateTexture(int width, int height) {
        std::cout << "[Texture] Creating custom texture\n";

            std::vector<unsigned char> data(width * height * 4);

            // This is a test code, the data will be feed by well the user (perhaps the fb)
            for (int i = 0; i < width * height; i++) {
                data[i * 4 + 0] = 255;
                data[i * 4 + 1] = 0;
                data[i * 4 + 2] = 255;
                data[i * 4 + 3] = 255;
            }

        glGenTextures(1, &m_Texture);
        glBindTexture(GL_TEXTURE_2D, m_Texture);

        std::cout << "[Texture] Generating custom texture (glTexImage2D)\n";

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        std::cout << "[Texture] Custom texture created\n";
    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_Texture);
    }
}
