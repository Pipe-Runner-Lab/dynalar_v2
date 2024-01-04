#include "spot_light.h"

SpotLight::SpotLight(const glm::vec3 color, float ambientIntensity, float diffuseIntensity,
                     float specularIntensity, glm::vec3 position, glm::vec3 direction, float cutoff,
                     float outerCutoff, float constant, float linear, float quadratic)
    : SpotLight("Spot Light", color, ambientIntensity, diffuseIntensity, specularIntensity,
                position, direction, cutoff, outerCutoff, constant, linear, quadratic) {
}

SpotLight::SpotLight(std::string name, const glm::vec3 color, float ambientIntensity,
                     float diffuseIntensity, float specularIntensity, glm::vec3 position,
                     glm::vec3 direction, float cutoff, float outerCutoff, float constant,
                     float linear, float quadratic)
    : BaseLight(name, LightType::SPOT, color, ambientIntensity,
                std::make_shared<Cube>(name, glm::vec3(0.0f), glm::vec3(0.5f),
                                       glm::vec4(color.r, color.g, color.b, 1.0f))),
      m_diffuseIntensity(diffuseIntensity),
      m_specularIntensity(specularIntensity),
      m_position(position),
      m_direction(direction),
      m_cutoff(cutoff),
      m_outerCutoff(outerCutoff),
      m_constant(constant),
      m_linear(linear),
      m_quadratic(quadratic) {
}

void SpotLight::Bind(Shader& shader, int idx) {
    shader.SetUniform3f(fmt::format("u_spotLights[{}].position", idx), m_position.x, m_position.y,
                        m_position.z);
    shader.SetUniform3f(fmt::format("u_spotLights[{}].direction", idx), m_direction.x,
                        m_direction.y, m_direction.z);
    shader.SetUniform3f(fmt::format("u_spotLights[{}].color", idx), m_color.r, m_color.g,
                        m_color.b);
    shader.SetUniform1f(fmt::format("u_spotLights[{}].ambientIntensity", idx), m_ambientIntensity);
    shader.SetUniform1f(fmt::format("u_spotLights[{}].diffuseIntensity", idx), m_diffuseIntensity);
    shader.SetUniform1f(fmt::format("u_spotLights[{}].specularIntensity", idx),
                        m_specularIntensity);

    shader.SetUniform1f(fmt::format("u_spotLights[{}].cutoff", idx), m_cutoff);
    shader.SetUniform1f(fmt::format("u_spotLights[{}].outerCutoff", idx), m_outerCutoff);

    shader.SetUniform1f(fmt::format("u_spotLights[{}].constant", idx), m_constant);
    shader.SetUniform1f(fmt::format("u_spotLights[{}].linear", idx), m_linear);
    shader.SetUniform1f(fmt::format("u_spotLights[{}].quadratic", idx), m_quadratic);
}

void SpotLight::Unbind(Shader& shader, int idx) {
    shader.SetUniform3f(fmt::format("u_spotLights[{}].position", idx), 0, 0, 0);
    shader.SetUniform3f(fmt::format("u_spotLights[{}].direction", idx), 0, 0, 0);
    shader.SetUniform3f(fmt::format("u_spotLights[{}].color", idx), 0, 0, 0);
    shader.SetUniform1f(fmt::format("u_spotLights[{}].ambientIntensity", idx), 0.0f);
    shader.SetUniform1f(fmt::format("u_spotLights[{}].diffuseIntensity", idx), 0.0f);
    shader.SetUniform1f(fmt::format("u_spotLights[{}].specularIntensity", idx), 0.0f);

    shader.SetUniform1f(fmt::format("u_spotLights[{}].cutoff", idx), 0.0f);
    shader.SetUniform1f(fmt::format("u_spotLights[{}].outerCutoff", idx), 0.0f);

    shader.SetUniform1f(fmt::format("u_spotLights[{}].constant", idx), 0.0f);
    shader.SetUniform1f(fmt::format("u_spotLights[{}].linear", idx), 0.0f);
    shader.SetUniform1f(fmt::format("u_spotLights[{}].quadratic", idx), 0.0f);
}

void SpotLight::Draw(Renderer& renderer, Shader& shader, glm::mat4& vpMatrix) {
    if (m_render_model) {
        m_lightModelPtr->m_position = m_position;
        m_lightModelPtr->Draw(renderer, shader, vpMatrix);
    }
}

void SpotLight::RenderEditorProperties() {
    ImGui::Text("Type: Spot Light");
    BaseLight::RenderEditorProperties();
    ImGui::DragFloat3("Position", glm::value_ptr(m_position), 0.1f);
    ImGui::DragFloat3("Direction", glm::value_ptr(m_direction), 0.1f);
    ImGui::SliderFloat("Diffuse Intensity", &m_diffuseIntensity, 0.0f, 1.0f);
    ImGui::SliderFloat("Specular Intensity", &m_specularIntensity, 0.0f, 1.0f);
    ImGui::SliderFloat("Constant", &m_constant, 0.0f, 1.0f);
    ImGui::SliderFloat("Linear", &m_linear, 0.0f, 1.0f);
    ImGui::SliderFloat("Quadratic", &m_quadratic, 0.0f, 1.0f);
    ImGui::SliderFloat("Cutoff Angle", &m_cutoff, 0.0f, 90.0f);
    ImGui::SliderFloat("Outer Cutoff Angle", &m_outerCutoff, 0.0f, 90.0f);
}
