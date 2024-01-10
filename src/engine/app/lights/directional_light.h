#include <glm/glm.hpp>

#include "../pre_made_objects/cube.h"
#include "base_light.h"

class DirectionalLight : public BaseLight {
    friend class BaseScene;

private:
    float m_diffuseIntensity;
    float m_specularIntensity;
    glm::vec3 m_direction;

    glm::mat4 m_projectionMatrix = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);

public:
    DirectionalLight(const glm::vec3 color, float ambientIntensity, float diffuseIntensity,
                     float specularIntensity, glm::vec3 direction);
    DirectionalLight(std::string name, const glm::vec3 color, float ambientIntensity,
                     float diffuseIntensity, float specularIntensity, glm::vec3 direction);

    void Bind(Shader& shader, int idx = 0) override;
    void Unbind(Shader& shader, int idx = 0) override;

    inline glm::mat4 GetVpMatrix() {
        glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0, 0, 0) - (40.0f * m_direction),
                                           glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        return m_projectionMatrix * viewMatrix;
    }

    /* ---------------------------- EDITOR PROPERTIES --------------------------- */
public:
    void RenderEditorProperties();
};