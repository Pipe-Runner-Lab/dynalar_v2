#pragma once

#include <GL/glew.h>
#include <fmt/core.h>

#include "../../window/window_manager.h"
#include "../utils/error.h"
#include "model.h"
#include "renderer.h"
#include "shader.h"

class DirectionalShadowMap {
    friend class DirectionalLight;

private:
    unsigned int m_depthMapFBO;
    unsigned int m_depthMap;

    int m_width;
    int m_height;

public:
    DirectionalShadowMap(int width = 2048, int height = 2048);
    ~DirectionalShadowMap();

    void Bind() const;
    void Unbind() const;

    void ActivateShadowTexture(int slot = 0) const;

    void GenerateShadow(Renderer& renderer, WindowManager& window_manager,
                        std::vector<std::unique_ptr<Model>>& modelPtrs, Shader& shader,
                        glm::mat4 vpMatrix) const;
};

class OmniDirectionalShadowMap {
    friend class PointLight;
    friend class SpotLight;

private:
    unsigned int m_depthMapFBO;
    unsigned int m_depthCubeMap;

    int m_width;
    int m_height;

public:
    OmniDirectionalShadowMap(int width = 1024, int height = 1024);
    ~OmniDirectionalShadowMap();

    void Bind() const;
    void Unbind() const;

    void ActivateShadowTexture(int slot = 0) const;
    void GenerateShadow(Renderer& renderer, WindowManager& window_manager,
                        std::vector<std::unique_ptr<Model>>& modelPtrs, Shader& shader,
                        std::array<glm::mat4, 6> vpMatrices) const;
};