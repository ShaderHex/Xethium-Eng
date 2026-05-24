#include "framebuffer/framebuffer.h"
#include "resourceManager/resourceManager.h"
#include "texture/texture.h"
#include <iostream>

namespace XENGINE {

namespace framebuffer {

void Framebuffer::Initialize(const FramebufferSpec& spec) {
    m_fbSpec = spec;

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    
    m_texture = XENGINE::ResourceManager::CreateBlankTexture(spec.width, spec.height);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->GetID(), 0); // attach it on to the 
    
    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_fbSpec.width, m_fbSpec.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "[Framebuffer] New framebuffer created and initialized with FBO address(memory): " << &m_fbo << " and FBO Value: " << m_fbo <<"\n";
    } else {
        std::cout << "[Framebuffer] Failed to initialize framebuffer!"<<"\n";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Making the framebuffer active
}

void Framebuffer::Update(const FramebufferSpec& spec) {
    std::cout << "[Framebuffer] Resize called!\n";
    m_fbSpec = spec;

    // Clearing up so leaks and random flashes don't happen
    if (m_fbo) {
        glDeleteFramebuffers(1, &m_fbo);
        m_fbo = 0;
    }
    if (m_rbo) {
        glDeleteRenderbuffers(1, &m_rbo);
        m_rbo = 0;
    }

    Initialize(spec);
    Bind();
}

void Framebuffer::Bind() {
    std::cout<< "[Framebuffer] Renderering the scene on the the framebuffer: " << this->GetTextureID() << "\n";
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glViewport(0, 0, m_fbSpec.width, m_fbSpec.height);
}

void Framebuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // TODO: Draw the framebuffer to a quadmesh
    // glBlitFramebuffer(0, 0, m_fbSpec.width, m_fbSpec.height, 
    //                   0, 0, m_fbSpec.width, m_fbSpec.height, 
    //                   GL_COLOR_BUFFER_BIT, GL_NEAREST); // for now we will copy the framebuffer and render from the copy not the actualy framebuffer itself as it requires external function that renders on quad mesh which i don't have right now
}

}

}