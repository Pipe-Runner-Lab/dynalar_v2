#include "shadow_map.h"

ShadowMap::ShadowMap(int width, int height) : m_width(width), m_height(height) {
    GL_CALL(glGenFramebuffers(1, &m_depthMapFBO));

    GL_CALL(glGenTextures(1, &m_depthMap));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, m_depthMap));
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT,
                         GL_FLOAT, nullptr));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};  // everything outside of the light's frustum
                                                     // will be in light (sideways)
    GL_CALL(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));

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

void ShadowMap::ActivateShadowTexture(int slot) const {
    GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, m_depthMap));
}

void ShadowMap::GenerateShadow(Renderer& renderer, WindowManager& window_manager,
                               std::vector<std::unique_ptr<Model>>& modelPtrs, Shader& shader,
                               glm::mat4 vpMatrix) const {
    window_manager.SetViewport(m_width, m_height);
    renderer.Clear(GL_DEPTH_BUFFER_BIT);
    // glCullFace(GL_FRONT); //TODO: Fix peter panning

    for (auto& modelPtr : modelPtrs) {
        modelPtr->Draw(renderer, shader, vpMatrix, true, RenderPassType::SHADOW);
    }

    // glCullFace(GL_BACK);
}
