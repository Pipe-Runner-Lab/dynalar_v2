#pragma once

#include "../../core/gl/model.h"

class Cube : public Model {
public:
    Cube(std::string title, glm::vec3 position = glm::vec3(0.0f),
         glm::vec3 rotation = glm::vec3(0.0f),
         glm::vec3 scale = glm::vec3(1.0f));
    ~Cube() {
    }
};
