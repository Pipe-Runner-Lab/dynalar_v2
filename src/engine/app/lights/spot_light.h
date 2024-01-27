#include <glm/glm.hpp>

#include "../pre_made_objects/cube.h"
#include "base_light.h"

class SpotLight : public BaseLight {
    friend class BaseScene;
    friend class LightsManager;

private:
    float m_diffuseIntensity;
    float m_specularIntensity;
    glm::vec3 m_position;
    // TODO: Add support for orientation
    glm::vec3 m_direction;
    float m_innerCutoff;  // deg
    float m_outerCutoff;  // deg

    // attenuation
    float m_constant;
    float m_linear;
    float m_quadratic;

    // shadow map
    bool m_shouldRenderShadowMap = true;
    DirectionalShadowMap m_shadowMap;
    float near = 0.1f;
    float far = 100.0f;

public:
    SpotLight(const glm::vec3 color, float ambientIntensity, float diffuseIntensity,
              float specularIntensity, glm::vec3 position, glm::vec3 direction, float cutoff,
              float outerCutoff, float constant = 1.0f, float linear = 0.09f,
              float quadratic = 0.032f);
    SpotLight(std::string name, const glm::vec3 color, float ambientIntensity,
              float diffuseIntensity, float specularIntensity, glm::vec3 position,
              glm::vec3 direction, float cutoff, float outerCutoff, float constant = 1.0f,
              float linear = 0.09f, float quadratic = 0.032f);

    void Bind(Shader& shader, int idx = 0, int shadowMapSlot = -1) override;
    void Unbind(Shader& shader, int idx = 0) override;

    void Draw(Renderer& renderer, Shader& shader, glm::mat4& vpMatrix) override;

    inline void SetPosition(const glm::vec3& position) {
        m_position = position;
    };

    inline void SetDirection(const glm::vec3& direction) {
        m_direction = direction;
    };

    void GenerateShadowMap(Renderer& renderer, WindowManager& window_manager,
                           std::vector<std::unique_ptr<Model>>& modelPtrs, Shader& shader);

    // for shadow mapping
    inline glm::mat4 GetVpMatrix() {
        float aspect = (float)m_shadowMap.m_width / (float)m_shadowMap.m_height;
        // TODO: Why glm::vec3(0, 0, 1) works and glm::vec3(0, 1, 0) for light pointing straight
        // down
        glm::mat4 viewMatrix =
            glm::lookAt(m_position, m_position + m_direction, glm::vec3(0, 0, 1));
        glm::mat4 m_projectionMatrix = glm::perspective(glm::radians(90.0f), aspect, near, far);
        return m_projectionMatrix * viewMatrix;
    }

    /* ---------------------------- EDITOR PROPERTIES --------------------------- */
public:
    void RenderEditorProperties();
};