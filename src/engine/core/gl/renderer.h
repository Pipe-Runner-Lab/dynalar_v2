#pragma once

#include <GL/glew.h>
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
};