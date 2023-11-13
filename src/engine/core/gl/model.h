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

    inline Model &Translate(glm::vec3 translation) {
        m_modelMatrix = glm::translate(m_modelMatrix, translation);
        return *this;
    }

    /// @brief Rotates the model along x, y, z axis. Rotation is applied in xyz
    /// order
    /// @param rotation A vec3 containing angles in degree for each axis
    inline Model &Rotate(glm::vec3 rotation) {
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(rotation.x),
                                    glm::vec3(1.0f, 0.0f, 0.0f));
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(rotation.y),
                                    glm::vec3(0.0f, 1.0f, 0.0f));
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(rotation.z),
                                    glm::vec3(0.0f, 0.0f, 1.0f));

        return *this;
    }

    void Draw(Renderer &renderer);

    inline glm::mat4 &GetModelMatrix() {
        return m_modelMatrix;
    }
};