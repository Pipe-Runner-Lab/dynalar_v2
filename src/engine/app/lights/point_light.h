#include <glm/glm.hpp>

#include "../pre_made_objects/cube.h"
#include "base_light.h"

class PointLight : public BaseLight {
    friend class BaseScene;

private:
    float m_diffuseIntensity;
    float m_specularIntensity;
    glm::vec3 m_position;

public:
    PointLight(const glm::vec3 color, float ambientIntensity,
               float diffuseIntensity, float specularIntensity,
               glm::vec3 position);
    PointLight(std::string name, const glm::vec3 color, float ambientIntensity,
               float diffuseIntensity, float specularIntensity,
               glm::vec3 position);

    void Bind(Shader& shader, int idx = 0) override;
    void Unbind(Shader& shader, int idx = 0) override;

    void Draw(Renderer& renderer, Shader& shader, glm::mat4& vpMatrix) override;
};