#pragma once

namespace Texture {

class Texture {
public:
    Texture(const char* path);
    void Bind(); 

private:
    int m_Width, m_Height, m_NrChannels;
    unsigned char *m_Data;
    unsigned m_Texture;
};

}
