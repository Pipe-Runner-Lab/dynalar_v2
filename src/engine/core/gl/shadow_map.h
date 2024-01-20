#pragma once

#include <GL/glew.h>
#include <fmt/core.h>

#include "../../window/window_manager.h"
#include "../utils/error.h"
#include "model.h"
#include "renderer.h"
#include "shader.h"

class ShadowMap {
    friend class DirectionalLight;
    friend class PointLight;
    friend class SpotLight;

private:
    unsigned int m_depthMapFBO;
    unsigned int m_depthMap;

    int m_width;
    int m_height;

public:
    ShadowMap(int width = 2048, int height = 2048);
    ~ShadowMap();

    void Bind() const;
    void Unbind() const;

    void ActivateShadowTexture(int slot = 0) const;

    void GenerateShadow(Renderer& renderer, WindowManager& window_manager,
                        std::vector<std::unique_ptr<Model>>& modelPtrs, Shader& shader,
                        glm::mat4 vpMatrix) const;
};