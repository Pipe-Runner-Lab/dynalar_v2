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

void DirectionalLight::Bind(Shader& shader, int idx, int shadowMapSlot) {
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
    shader.SetUniform1i(fmt::format("u_directionalLights[{}].shadowMapSlot", idx), shadowMapSlot);
}

void DirectionalLight::Unbind(Shader& shader, int idx) {
    shader.SetUniform3f(fmt::format("u_directionalLights[{}].direction", idx), 0, 0, 0);
    shader.SetUniform3f(fmt::format("u_directionalLights[{}].color", idx), 0, 0, 0);
    shader.SetUniform1f(fmt::format("u_directionalLights[{}].ambientIntensity", idx), 0.0f);
    shader.SetUniform1f(fmt::format("u_directionalLights[{}].diffuseIntensity", idx), 0.0f);
    shader.SetUniform1f(fmt::format("u_directionalLights[{}].specularIntensity", idx), 0.0f);
    shader.SetUniform1i(fmt::format("u_directionalLights[{}].shadowMapSlot", idx), -1);
}

void DirectionalLight::RenderEditorProperties() {
    ImGui::Text("Type: DirectionalLight Light");
    BaseLight::RenderEditorProperties();
    ImGui::DragFloat3("Direction", glm::value_ptr(m_direction), 0.1f);
    ImGui::SliderFloat("Diffuse Intensity", &m_diffuseIntensity, 0.0f, 1.0f);
    ImGui::SliderFloat("Specular Intensity", &m_specularIntensity, 0.0f, 1.0f);

    // shadow
    ImGui::Separator();
    ImGui::Text("Shadow Properties");
    ImGui::Text(
        fmt::format("Shadow Map Size: {} x {}", m_shadowMap.m_width, m_shadowMap.m_height).c_str());
    ImGui::Checkbox("Render Shadow Map", &m_shouldRenderShadowMap);
    ImGui::DragFloat("Width", &width, 0.1f, 0.01f);
    ImGui::DragFloat("Height", &height, 0.1f, 0.01f);
    ImGui::DragFloat("Near", &near, 0.1f);
    ImGui::DragFloat("Far", &far, 0.1f);
    ImGui::DragFloat("Aligned Position (From Origin)", &m_positionAlignedScale, 0.1f, 0.0f);
}

void DirectionalLight::Draw(Renderer& renderer, Shader& shader, glm::mat4& vpMatrix) {
    if (m_render_model) {
        m_lightModelPtr->m_position = m_position;
        m_lightModelPtr->Draw(renderer, shader, vpMatrix);
    }
}

void DirectionalLight::GenerateShadowMap(Renderer& renderer, WindowManager& window_manager,
                                         std::vector<std::unique_ptr<Model>>& modelPtrs,
                                         Shader& shader) {
    if (!m_shouldRenderShadowMap)
        return;

    m_shadowMap.Bind();
    m_shadowMap.GenerateShadow(renderer, window_manager, modelPtrs, shader, GetVpMatrix());
    m_shadowMap.Unbind();
}
