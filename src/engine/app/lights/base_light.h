#pragma once

#include <glm/glm.hpp>
#include <string>

#include "../../core/gl/model.h"
#include "../../core/gl/renderer.h"
#include "../../core/gl/shader.h"

enum class LightType {
    POINT,
    DIRECTIONAL,
    SPOT,
    AMBIENT,
};

class BaseLight {
    friend class BaseScene;

public:
    std::string name;
    LightType type;
    std::shared_ptr<Model> m_lightModelPtr = nullptr;

protected:
    bool m_render_model = false;
    glm::vec3 m_color;
    float m_ambientIntensity;

public:
    BaseLight(LightType type, const glm::vec3& color, float intensity,
              std::shared_ptr<Model> lightModelPtr = nullptr)
        : BaseLight("Light", type, color, intensity, lightModelPtr) {
    }

    BaseLight(std::string name, LightType type, const glm::vec3& color,
              float intensity, std::shared_ptr<Model> lightModelPtr = nullptr)
        : m_ambientIntensity(intensity),
          m_color(color),
          name(name),
          type(type),
          m_lightModelPtr(std::move(lightModelPtr)) {
    }

    inline float& GetAmbientIntensity() {
        return m_ambientIntensity;
    };

    virtual void Bind(Shader& shader, int idx = 0) = 0;
    virtual void Unbind(Shader& shader, int idx = 0) = 0;
    virtual void Draw(Renderer& renderer, Shader& shader, glm::mat4& vpMatrix) {
        if (m_lightModelPtr == nullptr || !m_render_model)
            return;
    };
};