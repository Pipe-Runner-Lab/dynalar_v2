#include <glm/glm.hpp>

#include "../pre_made_objects/cube.h"
#include "base_light.h"

class DirectionalLight : public BaseLight {
    friend class BaseScene;
    friend class LightsContainer;

private:
    float m_diffuseIntensity;
    float m_specularIntensity;
    glm::vec3 m_direction;

    // for shadow mapping
    bool m_shouldRenderShadowMap = true;
    ShadowMap m_shadowMap;
    glm::vec3 m_position;
    float left = -20.0f;
    float right = 20.0f;
    float bottom = -20.0f;
    float top = 20.0f;
    float near = 0.1f;
    float far = 100.0f;

    // glm::mat4 m_projectionMatrix = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);

public:
    DirectionalLight(const glm::vec3 color, float ambientIntensity, float diffuseIntensity,
                     float specularIntensity, glm::vec3 direction);
    DirectionalLight(std::string name, const glm::vec3 color, float ambientIntensity,
                     float diffuseIntensity, float specularIntensity, glm::vec3 direction);

    void Bind(Shader& shader, int idx = 0, int shadowMapSlot = -1) override;
    void Unbind(Shader& shader, int idx = 0) override;

    void Draw(Renderer& renderer, Shader& shader, glm::mat4& vpMatrix) override;

    void GenerateShadowMap(Renderer& renderer, WindowManager& window_manager,
                           std::vector<std::unique_ptr<Model>>& modelPtrs, Shader& shader);

    // for shadow mapping
    inline glm::mat4 GetVpMatrix() {
        glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0, 0, 0) - (40.0f * m_direction),
                                           glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 m_projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
        return m_projectionMatrix * viewMatrix;
    }

    /* ---------------------------- EDITOR PROPERTIES --------------------------- */
public:
    void RenderEditorProperties();
};