#include <glm/glm.hpp>

#include "../pre_made_objects/cube.h"
#include "base_light.h"

class PointLight : public BaseLight {
public:
    glm::vec3 position;

public:
    PointLight(const glm::vec3 color, float intensity, glm::vec3 position);
    PointLight(std::string name, const glm::vec3 color, float intensity,
               glm::vec3 position);

    void Bind(Shader& shader, int idx = 0) override;
    void Unbind(Shader& shader, int idx = 0) override;

    void Draw(Renderer& renderer, Shader& shader, glm::mat4& vpMatrix) override;
};