#include "ambient_light.h"

AmbientLight::AmbientLight(const glm::vec3 color, float intensity)
    : BaseLight("Ambient Light", LightType::AMBIENT_LIGHT, color, intensity) {
}

AmbientLight::AmbientLight(std::string name, const glm::vec3 color,
                           float intensity)
    : BaseLight(name, LightType::AMBIENT_LIGHT, color, intensity) {
}

void AmbientLight::Bind(Shader& shader) {
}

void AmbientLight::Unbind(Shader& shader) {
}
