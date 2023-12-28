#pragma once

#include <glm/glm.hpp>
#include <string>

#include "../../core/gl/model.h"
#include "../../core/gl/renderer.h"
#include "../../core/gl/shader.h"

enum class LightType {
    POINT_LIGHT,
    DIRECTIONAL_LIGHT,
    SPOT_LIGHT,
    AMBIENT_LIGHT,
};

class BaseLight {
public:
    std::string name;
    LightType type;
    bool render_model = false;
    std::shared_ptr<Model> m_lightModelPtr = nullptr;

protected:
    glm::vec3 m_color;
    float m_intensity;

public:
    BaseLight(LightType type, const glm::vec3& color, float intensity,
              std::shared_ptr<Model> lightModelPtr = nullptr)
        : BaseLight("Light", type, color, intensity, lightModelPtr) {
    }

    BaseLight(std::string name, LightType type, const glm::vec3& color,
              float intensity, std::shared_ptr<Model> lightModelPtr = nullptr)
        : m_intensity(intensity),
          m_color(color),
          name(name),
          type(type),
          m_lightModelPtr(std::move(lightModelPtr)) {
    }

    inline glm::vec3& GetColor() {
        return m_color;
    };
    inline float& GetIntensity() {
        return m_intensity;
    };

    virtual void Bind(Shader& shader, int idx = 0) = 0;
    virtual void Unbind(Shader& shader, int idx = 0) = 0;
    virtual void Draw(Renderer& renderer, Shader& shader, glm::mat4& vpMatrix) {
        if (m_lightModelPtr == nullptr || !render_model)
            return;
    };
};