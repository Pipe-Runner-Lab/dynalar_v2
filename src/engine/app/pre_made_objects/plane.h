#pragma once

#include "../../core/gl/model.h"

class Plane : public Model {
public:
    Plane(std::string title, glm::vec3 position = glm::vec3(0.0f),
          glm::vec3 rotation = glm::vec3(0.0f),
          glm::vec3 scale = glm::vec3(1.0f), glm::vec4 color = glm::vec4(1.0f));
    ~Plane() {
    }
};
