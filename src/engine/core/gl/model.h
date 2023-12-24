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
    glm::mat4 m_modelMatrix;

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

public:
    std::string title;
    bool debugNormals = false;

public:
    Model(std::string title, glm::vec3 position = glm::vec3(0.0f),
          glm::vec3 rotation = glm::vec3(0.0f),
          glm::vec3 scale = glm::vec3(1.0f));

    Model(std::string title, std::vector<Mesh> &meshGroup,
          glm::vec3 position = glm::vec3(0.0f),
          glm::vec3 rotation = glm::vec3(0.0f),
          glm::vec3 scale = glm::vec3(1.0f));

    ~Model();

    /// @brief Applies delta translation to the model
    /// @param translation
    /// @return the model itself
    inline Model &Translate(glm::vec3 translation) {
        m_position += translation;
        return *this;
    }

    /// @brief Sets the position of the model
    inline Model &SetPosition(glm::vec3 position) {
        m_position = position;
        return *this;
    }

    /// @brief Rotates the model by delta along x, y, z axis. Rotation is
    /// applied in xyz order
    /// @param rotation A vec3 containing angles in degree for each axis
    inline Model &Rotate(glm::vec3 rotation) {
        m_rotation += rotation;
        return *this;
    }

    /// @brief Sets the rotation of the model
    inline Model &SetOrientation(glm::vec3 rotation) {
        m_rotation = rotation;
        return *this;
    }

    inline Model &Scale(glm::vec3 scale) {
        m_scale += scale;
        return *this;
    }

    inline Model &SetScale(glm::vec3 scale) {
        m_scale = scale;
        return *this;
    }

    void Draw(Renderer &renderer);

    inline glm::mat4 &GetModelMatrix() {
        m_modelMatrix = glm::mat4(1.0f);
        m_modelMatrix = glm::translate(m_modelMatrix, m_position);
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.x),
                                    glm::vec3(1.0f, 0.0f, 0.0f));
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.y),
                                    glm::vec3(0.0f, 1.0f, 0.0f));
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.z),
                                    glm::vec3(0.0f, 0.0f, 1.0f));
        m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
        return m_modelMatrix;
    }

    inline glm::vec3 &GetPosition() {
        return m_position;
    }

    inline glm::vec3 &GetRotation() {
        return m_rotation;
    }

    inline glm::vec3 &GetScale() {
        return m_scale;
    }

    static void PhongShadingConverter(std::vector<Mesh> &meshGroup){};
};