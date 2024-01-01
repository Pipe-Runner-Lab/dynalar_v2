#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "model.h"

class Camera {
    friend class BaseScene;

private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_worldUp;
    glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);

    float m_yaw;
    float m_pitch;

    glm::mat4 m_viewMatrix = glm::mat4(1.0f);
    glm::mat4 m_projectionMatrix;

public:
    Camera(glm::vec3 starPosition, float startPitch, float startYaw,
           glm::mat4 perspectiveMatrix);

    // TODO
    Camera(glm::vec3 starPosition, glm::vec3 lookAt);

    // TODO set dir and compute pitch and yaw
    void LookAt(Model &model);

    void UpdateDirection(float deltaPitch, float deltaYaw);
    void UpdatePosition(glm::vec3 deltaPosition);
    void LookAt(float pitch, float yaw);

    inline glm::vec3 &GetFront() {
        return m_front;
    }

    inline glm::vec3 &GetUp() {
        return m_up;
    }

    inline glm::vec3 &GetRight() {
        return m_right;
    }

    inline glm::vec3 &GetPosition() {
        return m_position;
    }

    glm::mat4 &GetViewMatrix();
    glm::mat4 &GetProjectionMatrix();
};