#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "model.h"

class Camera
{
private:
  glm::vec3 m_position;
  glm::vec3 m_front;
  glm::vec3 m_up;
  glm::vec3 m_worldUp;

  float m_yaw;
  float m_pitch;

  glm::mat4 m_viewMatrix;
  glm::mat4 m_perspectiveMatrix;

public:
  Camera(glm::vec3 starPosition, glm::vec3 startUp, float startPitch, float startYaw, glm::mat4 perspectiveMatrix);

  // TODO
  Camera(glm::vec3 starPosition, glm::vec3 lookAt);

  // TODO
  void LookAt(Model &model);

  glm::mat4 &GetViewMatrix();
  glm::mat4 &GetProjectionMatrix();
};