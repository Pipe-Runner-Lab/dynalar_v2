#include <glm/glm.hpp>

#include "../pre_made_objects/cube.h"
#include "base_light.h"

class SpotLight : public BaseLight {
    friend class BaseScene;

private:
    float m_diffuseIntensity;
    float m_specularIntensity;
    glm::vec3 m_position;
    // TODO: Add support for orientation
    glm::vec3 m_direction;
    float m_cutoff;       // deg
    float m_outerCutoff;  // deg

    // attenuation
    float m_constant;
    float m_linear;
    float m_quadratic;

public:
    SpotLight(const glm::vec3 color, float ambientIntensity, float diffuseIntensity,
              float specularIntensity, glm::vec3 position, glm::vec3 direction, float cutoff,
              float outerCutoff, float constant = 1.0f, float linear = 0.09f,
              float quadratic = 0.032f);
    SpotLight(std::string name, const glm::vec3 color, float ambientIntensity,
              float diffuseIntensity, float specularIntensity, glm::vec3 position,
              glm::vec3 direction, float cutoff, float outerCutoff, float constant = 1.0f,
              float linear = 0.09f, float quadratic = 0.032f);

    void Bind(Shader& shader, int idx = 0) override;
    void Unbind(Shader& shader, int idx = 0) override;

    void Draw(Renderer& renderer, Shader& shader, glm::mat4& vpMatrix) override;

    inline void SetPosition(const glm::vec3& position) {
        m_position = position;
    };

    inline void SetDirection(const glm::vec3& direction) {
        m_direction = direction;
    };

    /* ---------------------------- EDITOR PROPERTIES --------------------------- */
public:
    void RenderEditorProperties();
};