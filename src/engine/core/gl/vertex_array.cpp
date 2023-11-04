#include "vertex_array.h"

VertexArray::VertexArray() : m_arrayID(0)
{
  GL_CALL(glGenVertexArrays(1, &m_arrayID));
}

VertexArray::~VertexArray()
{
  GL_CALL(glDeleteVertexArrays(1, &m_arrayID));
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
  Bind();
  vb.Bind();
  const auto &elements = layout.GetElements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++)
  {
    const auto &element = elements[i];
    GL_CALL(glEnableVertexAttribArray(i));
    GL_CALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void *)offset));
    offset += element.count * VertexBufferElement::GetSize(element.type);
  }
  vb.Unbind();
  Unbind();
}

void VertexArray::Bind() const
{
  GL_CALL(glBindVertexArray(m_arrayID));
}

void VertexArray::Unbind() const
{
  GL_CALL(glBindVertexArray(0));
}