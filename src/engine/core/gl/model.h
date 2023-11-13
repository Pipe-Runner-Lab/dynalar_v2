#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "mesh.h"
#include "renderer.h"

class Model {
protected:
    std::vector<Mesh> m_meshes;
    glm::mat4 m_modelMatrix = glm::mat4(1.0f);

public:
    Model(){};
    Model(std::vector<Mesh> &meshGroup);
    ~Model();
    void Draw(Renderer &renderer);

    inline glm::mat4 &GetModelMatrix() {
        m_modelMatrix = glm::mat4(1.0f);
        return m_modelMatrix;
    }
};