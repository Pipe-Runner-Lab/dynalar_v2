#include "vertex_buffer.h"

VertexBuffer::VertexBuffer(const void *data, GLuint size) : m_bufferID(0)
{
  GL_CALL(glGenBuffers(1, &m_bufferID));
  Bind();
  GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
  GL_CALL(glDeleteBuffers(1, &m_bufferID));
}

void VertexBuffer::Bind() const
{
  GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_bufferID));
}

void VertexBuffer::Unbind() const
{
  GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}