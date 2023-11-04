#include "vertex_buffer_layout.h"

VertexBufferLayout::VertexBufferLayout() : m_stride(0) {}

unsigned int VertexBufferElement::GetSize(GLuint type)
{
  switch (type)
  {
  case (GL_FLOAT):
    return 4;
  case (GL_UNSIGNED_INT):
    return 4;
  case (GL_UNSIGNED_BYTE):
    return 1;
  }

  throw "Unknown type";
}