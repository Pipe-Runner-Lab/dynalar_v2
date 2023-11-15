#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <stdexcept>
#include <vector>

#include "mesh.h"
#include "renderer.h"

class Model {
protected:
    std::vector<Mesh> m_meshes;
    glm::mat4 m_modelMatrix = glm::mat4(1.0f);

    // initial model matrix
    glm::mat4 m_initialModelMatrix = m_modelMatrix;
    glm::mat4 m_initialModelMatrixInverse = m_modelMatrix;

    float tx;
    float ty;
    float tz;

    float rx;
    float ry;
    float rz;

    float sx;
    float sy;
    float sz;

public:
    Model(glm::vec3 position = glm::vec3(0.0f),
          glm::vec3 rotation = glm::vec3(0.0f),
          glm::vec3 scale = glm::vec3(1.0f));
    Model(std::vector<Mesh> &meshGroup, glm::vec3 position = glm::vec3(0.0f),
          glm::vec3 rotation = glm::vec3(0.0f),
          glm::vec3 scale = glm::vec3(1.0f));
    ~Model();

    inline Model &Translate(glm::vec3 translation) {
        tx = translation.x;
        ty = translation.y;
        tz = translation.z;

        m_modelMatrix *= m_initialModelMatrixInverse;
        m_modelMatrix = glm::translate(m_modelMatrix, translation);
        m_modelMatrix *= m_initialModelMatrix;

        return *this;
    }

    /// @brief Rotates the model along x, y, z axis. Rotation is applied in xyz
    /// order
    /// @param rotation A vec3 containing angles in degree for each axis
    inline Model &Rotate(glm::vec3 rotation) {
        rx = rotation.x;
        ry = rotation.y;
        rz = rotation.z;

        fmt::print("rx: {}, ry: {}, rz: {}\n", rx, ry, rz);

        m_modelMatrix = m_initialModelMatrixInverse * m_modelMatrix;
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(rotation.x),
                                    glm::vec3(1.0f, 0.0f, 0.0f));
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(rotation.y),
                                    glm::vec3(0.0f, 1.0f, 0.0f));
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(rotation.z),
                                    glm::vec3(0.0f, 0.0f, 1.0f));
        m_modelMatrix = m_initialModelMatrix * m_modelMatrix;

        return *this;
    }

    inline Model &Scale(glm::vec3 scale) {
        sx = scale.x;
        sy = scale.y;
        sz = scale.z;

        m_modelMatrix *= m_initialModelMatrixInverse;
        m_modelMatrix = glm::scale(m_modelMatrix, scale);
        m_modelMatrix *= m_initialModelMatrix;

        return *this;
    }

    void Draw(Renderer &renderer);

    inline glm::mat4 &GetModelMatrix() {
        return m_modelMatrix;
    }

    inline glm::vec3 GetPosition() {
        return glm::vec3(tx, ty, tz);
    }

    inline glm::vec3 GetRotation() {
        return glm::vec3(rx, ry, rz);
    }

    inline glm::vec3 GetScale() {
        return glm::vec3(sx, sy, sz);
    }

private:
    void InitializeModelMatrix(glm::vec3 &position, glm::vec3 &rotation,
                               glm::vec3 &scale);
};