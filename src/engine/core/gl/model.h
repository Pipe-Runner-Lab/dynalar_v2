#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <stdexcept>
#include <vector>

#include "mesh.h"
#include "renderer.h"

/**
 * Note: Rotation is right-handed
 */
class Model {
protected:
    std::shared_ptr<std::vector<Mesh>> m_meshesPtr =
        std::make_shared<std::vector<Mesh>>();
    glm::mat4 m_modelMatrix = glm::mat4(1.0f);

    // initial model matrix
    glm::mat4 m_initialModelMatrix = glm::mat4(1.0f);
    glm::mat4 m_initialModelMatrixInverse = glm::mat4(1.0f);

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

public:
    bool debugNormals = false;

public:
    Model(glm::vec3 position = glm::vec3(0.0f),
          glm::vec3 rotation = glm::vec3(0.0f),
          glm::vec3 scale = glm::vec3(1.0f));
    Model(std::vector<Mesh> &meshGroup, glm::vec3 position = glm::vec3(0.0f),
          glm::vec3 rotation = glm::vec3(0.0f),
          glm::vec3 scale = glm::vec3(1.0f));
    ~Model();

    /// @brief Applies delta translation to the model
    /// @param translation
    /// @return the model itself
    inline Model &Translate(glm::vec3 translation) {
        m_position += translation;

        m_modelMatrix *= m_initialModelMatrixInverse;
        m_modelMatrix = glm::translate(m_modelMatrix, translation);
        m_modelMatrix *= m_initialModelMatrix;

        return *this;
    }

    /// @brief Sets the position of the model
    inline Model &SetPosition(glm::vec3 position) {
        InitializeModelMatrix(position, m_rotation, m_scale, true);
        return *this;
    }

    /// @brief Rotates the model by delta along x, y, z axis. Rotation is
    /// applied in xyz order
    /// @param rotation A vec3 containing angles in degree for each axis
    inline Model &Rotate(glm::vec3 rotation) {
        m_rotation += rotation;

        // TODO: verify this
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

    /// @brief Sets the rotation of the model
    inline Model &SetOrientation(glm::vec3 rotation) {
        InitializeModelMatrix(m_position, rotation, m_scale, true);
        return *this;
    }

    inline Model &Scale(glm::vec3 scale) {
        m_scale += scale;

        m_modelMatrix *= m_initialModelMatrixInverse;
        m_modelMatrix = glm::scale(m_modelMatrix, scale);
        m_modelMatrix *= m_initialModelMatrix;

        return *this;
    }

    inline Model &SetScale(glm::vec3 scale) {
        InitializeModelMatrix(m_position, m_rotation, scale, true);
        return *this;
    }

    void Draw(Renderer &renderer);

    inline glm::mat4 &GetModelMatrix() {
        return m_modelMatrix;
    }

    // TODO: Check const usage
    inline glm::vec3 const &GetPosition() {
        return m_position;
    }

    inline glm::vec3 const &GetRotation() {
        return m_rotation;
    }

    inline glm::vec3 const &GetScale() {
        return m_scale;
    }

    static void PhongShadingConverter(std::vector<Mesh> &meshGroup){};

private:
    void InitializeModelMatrix(glm::vec3 &position, glm::vec3 &rotation,
                               glm::vec3 &scale, bool forceReset = false);
};