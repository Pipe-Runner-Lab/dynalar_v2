#include "point_light.h"

PointLight::PointLight(const glm::vec3 color, float ambientIntensity, float diffuseIntensity,
                       float specularIntensity, glm::vec3 position, float constant, float linear,
                       float quadratic)
    : PointLight("Point Light", color, ambientIntensity, diffuseIntensity, specularIntensity,
                 position, constant, linear, quadratic) {
}

PointLight::PointLight(std::string name, const glm::vec3 color, float ambientIntensity,
                       float diffuseIntensity, float specularIntensity, glm::vec3 position,
                       float constant, float linear, float quadratic)
    : BaseLight(name, LightType::POINT, color, ambientIntensity,
                std::make_shared<Cube>(name, glm::vec3(0.0f), glm::vec3(0.5f),
                                       glm::vec4(color.r, color.g, color.b, 1.0f))),
      m_diffuseIntensity(diffuseIntensity),
      m_specularIntensity(specularIntensity),
      m_position(position),
      m_constant(constant),
      m_linear(linear),
      m_quadratic(quadratic) {
}

void PointLight::Bind(Shader& shader, int idx, int shadowMapSlot) {
    shader.SetUniform3f(fmt::format("u_pointLights[{}].position", idx), m_position.x, m_position.y,
                        m_position.z);
    shader.SetUniform3f(fmt::format("u_pointLights[{}].color", idx), m_color.r, m_color.g,
                        m_color.b);
    shader.SetUniform1f(fmt::format("u_pointLights[{}].ambientIntensity", idx), m_ambientIntensity);
    shader.SetUniform1f(fmt::format("u_pointLights[{}].diffuseIntensity", idx), m_diffuseIntensity);
    shader.SetUniform1f(fmt::format("u_pointLights[{}].specularIntensity", idx),
                        m_specularIntensity);

    shader.SetUniform1f(fmt::format("u_pointLights[{}].constant", idx), m_constant);
    shader.SetUniform1f(fmt::format("u_pointLights[{}].linear", idx), m_linear);
    shader.SetUniform1f(fmt::format("u_pointLights[{}].quadratic", idx), m_quadratic);

    shader.SetUniform1i(fmt::format("u_pointLights[{}].shadowMapSlot", idx), shadowMapSlot);
}

void PointLight::Unbind(Shader& shader, int idx) {
    shader.SetUniform3f(fmt::format("u_pointLights[{}].position", idx), 0, 0, 0);
    shader.SetUniform3f(fmt::format("u_pointLights[{}].color", idx), 0, 0, 0);
    shader.SetUniform1f(fmt::format("u_pointLights[{}].ambientIntensity", idx), 0.0f);
    shader.SetUniform1f(fmt::format("u_pointLights[{}].diffuseIntensity", idx), 0.0f);
    shader.SetUniform1f(fmt::format("u_pointLights[{}].specularIntensity", idx), 0.0f);

    shader.SetUniform1f(fmt::format("u_pointLights[{}].constant", idx), 0.0f);
    shader.SetUniform1f(fmt::format("u_pointLights[{}].linear", idx), 0.0f);
    shader.SetUniform1f(fmt::format("u_pointLights[{}].quadratic", idx), 0.0f);

    shader.SetUniform1i(fmt::format("u_pointLights[{}].shadowMapSlot", idx), -1);
}

void PointLight::Draw(Renderer& renderer, Shader& shader, glm::mat4& vpMatrix) {
    if (m_render_model) {
        m_lightModelPtr->m_position = m_position;
        m_lightModelPtr->Draw(renderer, shader, vpMatrix);
    }
}

void PointLight::RenderEditorProperties() {
    ImGui::Text("Type: PointLight Light");
    BaseLight::RenderEditorProperties();
    ImGui::DragFloat3("Position", glm::value_ptr(m_position), 0.1f);
    ImGui::SliderFloat("Diffuse Intensity", &m_diffuseIntensity, 0.0f, 1.0f);
    ImGui::SliderFloat("Specular Intensity", &m_specularIntensity, 0.0f, 1.0f);
    ImGui::SliderFloat("Constant", &m_constant, 0.0f, 1.0f);
    ImGui::SliderFloat("Linear", &m_linear, 0.0f, 1.0f);
    ImGui::SliderFloat("Quadratic", &m_quadratic, 0.0f, 1.0f);
}
