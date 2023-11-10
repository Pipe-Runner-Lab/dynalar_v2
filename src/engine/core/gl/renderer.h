#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "vertex_array.h"
#include "index_buffer.h"
#include "../utils/error.h"

class Renderer
{
public:
  Renderer();

  void Clear();
  void Clear(float r, float g, float b, float a);
  void Draw(VertexArray &vertexArray, IndexBuffer &indexBuffer);

  // TODO: return by value is not efficient
  static glm::mat4 ComputeMVPMatrix(glm::mat4 &modelMatrix, glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix);
};