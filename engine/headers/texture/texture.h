#pragma once
#include "glad/glad.h"
#include "../../include/defines.h"
#include <memory>

namespace Texture {
    
class Texture {
public:
    Texture(const char* path);
    ~Texture();
    void Bind();
    void CreateTexture(int width, int height);

private:
    int m_Width = 0, m_Height = 0, m_NrChannels = 0;
    unsigned char *m_Data = nullptr;
    unsigned m_Texture = 0;
};

}
