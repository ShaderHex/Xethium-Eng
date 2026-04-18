#pragma once
#include <memory>
#include "glad/glad.h"
#include "texture/texture.h"

namespace XENGINE {

namespace framebuffer {

struct FramebufferSpec {
    int height = 0, width = 0;
};

class Framebuffer {
public:
    void Initialize(const FramebufferSpec& spec);
    void Bind();
    void Unbind();

    unsigned int GetID() const { return m_fbo; }

private:
    unsigned int m_fbo = 0;
    unsigned int m_textureColorBuffer = 0;
    std::shared_ptr<Texture::Texture> m_texture;
    FramebufferSpec m_fbSpec;
};

}

}