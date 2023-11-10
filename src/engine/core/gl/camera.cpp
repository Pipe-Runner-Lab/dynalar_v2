#include "camera.h"

Camera::Camera(glm::vec3 starPosition, glm::vec3 startUp, float startPitch, float startYaw, glm::mat4 perspectiveMatrix) : m_position(starPosition),
                                                                                                                           m_up(startUp),
                                                                                                                           m_pitch(startPitch),
                                                                                                                           m_yaw(startYaw),
                                                                                                                           m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
                                                                                                                           m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
                                                                                                                           m_perspectiveMatrix(perspectiveMatrix),
                                                                                                                           m_viewMatrix(glm::lookAt(m_position, m_position + m_front, m_up))
{
}

glm::mat4 &Camera::GetViewMatrix()
{
  return m_viewMatrix;
}

glm::mat4 &Camera::GetProjectionMatrix()
{
  return m_perspectiveMatrix;
}
