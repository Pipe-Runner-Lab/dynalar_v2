#pragma once

#include <GL/glew.h>
#include "../utils/error.h"

class VertexBuffer
{
private:
  GLuint m_bufferID;

public:
  VertexBuffer(const void *data, GLuint size);
  ~VertexBuffer();

  void Bind() const;
  void Unbind() const;
};
