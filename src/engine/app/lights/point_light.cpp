#include "point_light.h"

PointLight::PointLight(const glm::vec3 color, float intensity,
                       glm::vec3 position)
    : PointLight("Point Light", color, intensity, position) {
}

PointLight::PointLight(std::string name, const glm::vec3 color, float intensity,
                       glm::vec3 position)
    : BaseLight(
          name, LightType::POINT, color, intensity,
          std::make_shared<Cube>(name, glm::vec3(0.0f), glm::vec3(0.5f),
                                 glm::vec4(color.r, color.g, color.b, 1.0f))),
      position(position) {
}

void PointLight::Bind(Shader& shader, int idx) {
    shader.SetUniform3f(fmt::format("u_pointLights[{}].position", idx),
                        position.x, position.y, position.z);
    shader.SetUniform3f(fmt::format("u_pointLights[{}].color", idx), m_color.r,
                        m_color.g, m_color.b);
    shader.SetUniform1f(fmt::format("u_pointLights[{}].intensity", idx),
                        m_intensity);
}

void PointLight::Unbind(Shader& shader, int idx) {
    shader.SetUniform3f(fmt::format("u_pointLights[{}].position", idx), 0, 0,
                        0);
    shader.SetUniform3f(fmt::format("u_pointLights[{}].color", idx), 0, 0, 0);
    shader.SetUniform1f(fmt::format("u_pointLights[{}].intensity", idx), 0.0f);
}

void PointLight::Draw(Renderer& renderer, Shader& shader, glm::mat4& vpMatrix) {
    if (render_model) {
        m_lightModelPtr->GetPosition() = position;
        m_lightModelPtr->Draw(renderer, shader, vpMatrix);
    }
}
