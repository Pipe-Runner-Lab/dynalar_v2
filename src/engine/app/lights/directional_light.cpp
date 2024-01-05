#include "directional_light.h"

DirectionalLight::DirectionalLight(const glm::vec3 color, float ambientIntensity,
                                   float diffuseIntensity, float specularIntensity,
                                   glm::vec3 direction)
    : DirectionalLight("Directional Light", color, ambientIntensity, diffuseIntensity,
                       specularIntensity, direction) {
}

DirectionalLight::DirectionalLight(std::string name, const glm::vec3 color, float ambientIntensity,
                                   float diffuseIntensity, float specularIntensity,
                                   glm::vec3 direction)
    : BaseLight(name, LightType::DIRECTIONAL, color, ambientIntensity,
                std::make_shared<Cube>(name, glm::vec3(0.0f), glm::vec3(0.5f),
                                       glm::vec4(color.r, color.g, color.b, 1.0f))),
      m_diffuseIntensity(diffuseIntensity),
      m_specularIntensity(specularIntensity),
      m_direction(direction) {
}

void DirectionalLight::Bind(Shader& shader, int idx) {
    shader.SetUniform3f(fmt::format("u_directionalLights[{}].direction", idx), m_direction.x,
                        m_direction.y, m_direction.z);
    shader.SetUniform3f(fmt::format("u_directionalLights[{}].color", idx), m_color.r, m_color.g,
                        m_color.b);
    shader.SetUniform1f(fmt::format("u_directionalLights[{}].ambientIntensity", idx),
                        m_ambientIntensity);
    shader.SetUniform1f(fmt::format("u_directionalLights[{}].diffuseIntensity", idx),
                        m_diffuseIntensity);
    shader.SetUniform1f(fmt::format("u_directionalLights[{}].specularIntensity", idx),
                        m_specularIntensity);
}

void DirectionalLight::Unbind(Shader& shader, int idx) {
    shader.SetUniform3f(fmt::format("u_directionalLights[{}].direction", idx), 0, 0, 0);
    shader.SetUniform3f(fmt::format("u_directionalLights[{}].color", idx), 0, 0, 0);
    shader.SetUniform1f(fmt::format("u_directionalLights[{}].ambientIntensity", idx), 0.0f);
    shader.SetUniform1f(fmt::format("u_directionalLights[{}].diffuseIntensity", idx), 0.0f);
    shader.SetUniform1f(fmt::format("u_directionalLights[{}].specularIntensity", idx), 0.0f);
}

void DirectionalLight::RenderEditorProperties() {
    ImGui::Text("Type: DirectionalLight Light");
    BaseLight::RenderEditorProperties();
    ImGui::DragFloat3("Direction", glm::value_ptr(m_direction), 0.1f);
    ImGui::SliderFloat("Diffuse Intensity", &m_diffuseIntensity, 0.0f, 1.0f);
    ImGui::SliderFloat("Specular Intensity", &m_specularIntensity, 0.0f, 1.0f);
}
