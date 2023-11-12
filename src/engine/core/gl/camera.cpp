#include "camera.h"

Camera::Camera(glm::vec3 starPosition, float startPitch, float startYaw,
               glm::mat4 projectionMatrix)
    : m_position(starPosition),
      m_up(glm::vec3(1.0f)),
      m_front(glm::vec3(1.0f)),
      m_pitch(startPitch),
      m_yaw(startYaw),
      m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
      m_projectionMatrix(projectionMatrix),
      m_viewMatrix(glm::lookAt(m_position, m_position + m_front, m_up)) {
    LookAt(m_pitch, m_yaw);
}

void Camera::LookAt(float pitch, float yaw) {
    m_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    m_front.y = sin(glm::radians(pitch));
    m_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    m_front = glm::normalize(m_front);

    glm::vec3 right = glm::normalize(glm::cross(m_front, m_worldUp));

    m_up = glm::normalize(glm::cross(right, m_front));
}

glm::mat4 &Camera::GetViewMatrix() {
    return m_viewMatrix;
}

glm::mat4 &Camera::GetProjectionMatrix() {
    return m_projectionMatrix;
}
