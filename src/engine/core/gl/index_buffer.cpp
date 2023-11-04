#include "index_buffer.h"

IndexBuffer::IndexBuffer(const GLuint *data, GLuint count) : m_bufferID(0), m_Count(count)
{
  GL_CALL(glGenBuffers(1, &m_bufferID));

  Bind();
  GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
  Unbind();
}

IndexBuffer::~IndexBuffer()
{
  GL_CALL(glDeleteBuffers(1, &m_bufferID));
}

void IndexBuffer::Bind() const
{
  GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID));
}

void IndexBuffer::Unbind() const
{
  GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}