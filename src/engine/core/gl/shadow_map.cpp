#include "shadow_map.h"

ShadowMap::ShadowMap(int width, int height) : m_width(width), m_height(height) {
    GL_CALL(glGenFramebuffers(1, &m_depthMapFBO));

    GL_CALL(glGenTextures(1, &m_depthMap));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, m_depthMap));
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT,
                         GL_FLOAT, nullptr));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    // attach depth texture as FBO's depth buffer
    Bind();
    GL_CALL(
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0));
    GL_CALL(glDrawBuffer(GL_NONE));
    GL_CALL(glReadBuffer(GL_NONE));
    Unbind();
}

ShadowMap::~ShadowMap() {
    Unbind();
    GL_CALL(glDeleteFramebuffers(1, &m_depthMapFBO));
    GL_CALL(glDeleteTextures(1, &m_depthMap));
}

void ShadowMap::Bind() const {
    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO));
}

void ShadowMap::Unbind() const {
    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void ShadowMap::Draw(Renderer& renderer, WindowManager& window_manager) const {
    window_manager.SetViewport(m_width, m_height);
    renderer.Clear(GL_DEPTH_BUFFER_BIT);
}
