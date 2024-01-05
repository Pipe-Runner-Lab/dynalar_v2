#include <glm/glm.hpp>

#include "../pre_made_objects/cube.h"
#include "base_light.h"

class DirectionalLight : public BaseLight {
    friend class BaseScene;

private:
    float m_diffuseIntensity;
    float m_specularIntensity;
    glm::vec3 m_direction;

public:
    DirectionalLight(const glm::vec3 color, float ambientIntensity, float diffuseIntensity,
                     float specularIntensity, glm::vec3 direction);
    DirectionalLight(std::string name, const glm::vec3 color, float ambientIntensity,
                     float diffuseIntensity, float specularIntensity, glm::vec3 direction);

    void Bind(Shader& shader, int idx = 0) override;
    void Unbind(Shader& shader, int idx = 0) override;

    /* ---------------------------- EDITOR PROPERTIES --------------------------- */
public:
    void RenderEditorProperties();
};