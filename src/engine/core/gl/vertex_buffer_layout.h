#pragma once

#include <vector>
#include <stdexcept>
#include <GL/glew.h>
#include "../utils/error.h"

struct VertexBufferElement
{
  GLuint count;
  GLuint type;
  GLboolean normalized;

  static unsigned int GetSize(GLuint type);
};

class VertexBufferLayout
{
private:
  std::vector<VertexBufferElement> m_VertexBufferElements;
  GLuint m_stride;

public:
  VertexBufferLayout();

  /*
    Each push is done for one type of item
    push(3) 3 for vertex
    push(3) 3 for color
    push(3) 3 for normal vector

    Thus one entine set if of 9 items, this is why stride is updated with +=
  */
  template <typename T>
  void Push(GLuint count)
  {
    throw "Unknown type";
  }

  template <>
  void Push<float>(GLuint count)
  {
    m_VertexBufferElements.push_back(VertexBufferElement{count, GL_FLOAT, GL_FALSE});
    m_stride += count * VertexBufferElement::GetSize(GL_FLOAT);
  }

  template <>
  void Push<unsigned int>(GLuint count)
  {
    m_VertexBufferElements.push_back(VertexBufferElement{count, GL_UNSIGNED_INT, GL_FALSE});
    m_stride += count * VertexBufferElement::GetSize(GL_UNSIGNED_INT);
  }

  template <>
  void Push<unsigned char>(GLuint count)
  {
    m_VertexBufferElements.push_back(VertexBufferElement{count, GL_UNSIGNED_BYTE, GL_TRUE});
    m_stride += count * VertexBufferElement::GetSize(GL_UNSIGNED_BYTE);
  }

  inline const std::vector<VertexBufferElement> &GetElements() const
  {
    return m_VertexBufferElements;
  };

  /**
   * @brief Returns the stride for the data layout
   * @return layout in bytes
   */
  inline GLuint GetStride() const
  {
    return m_stride;
  };
};
