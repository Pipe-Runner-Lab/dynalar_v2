#include "ambient_light.h"

AmbientLight::AmbientLight(const glm::vec3 color, float intensity)
    : AmbientLight("Ambient Light", color, intensity) {
}

AmbientLight::AmbientLight(std::string name, const glm::vec3 color,
                           float intensity)
    : BaseLight(name, LightType::AMBIENT, color, intensity) {
}

void AmbientLight::Bind(Shader& shader, int idx) {
    shader.SetUniform3f("u_ambientLight.color", m_color.r, m_color.g,
                        m_color.b);
    shader.SetUniform1f("u_ambientLight.ambientIntensity", m_ambientIntensity);
}

void AmbientLight::Unbind(Shader& shader, int idx) {
}
