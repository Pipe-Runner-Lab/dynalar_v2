#include "vertex_buffer_layout.h"

VertexBufferLayout::VertexBufferLayout() : m_stride(0) {
}

unsigned int VertexBufferElement::GetSize(GLuint type) {
    switch (type) {
        case (GL_FLOAT):
            return 4;
        case (GL_UNSIGNED_INT):
            return 4;
        case (GL_UNSIGNED_BYTE):
            return 1;
    }

    throw "Unknown type";
}

template <>
void VertexBufferLayout::Push<float>(GLuint count) {
    m_VertexBufferElements.push_back(
        VertexBufferElement{count, GL_FLOAT, GL_FALSE});
    m_stride += count * VertexBufferElement::GetSize(GL_FLOAT);
}

template <>
void VertexBufferLayout::Push<unsigned int>(GLuint count) {
    m_VertexBufferElements.push_back(
        VertexBufferElement{count, GL_UNSIGNED_INT, GL_FALSE});
    m_stride += count * VertexBufferElement::GetSize(GL_UNSIGNED_INT);
}

template <>
void VertexBufferLayout::Push<unsigned char>(GLuint count) {
    m_VertexBufferElements.push_back(
        VertexBufferElement{count, GL_UNSIGNED_BYTE, GL_TRUE});
    m_stride += count * VertexBufferElement::GetSize(GL_UNSIGNED_BYTE);
}