#pragma once

#include "base_light.h"

class AmbientLight : public BaseLight {
public:
    AmbientLight(const glm::vec3 color, float intensity);

    AmbientLight(std::string name, const glm::vec3 color, float intensity);

    void Bind(Shader& shader) override;
    void Unbind(Shader& shader) override;
};