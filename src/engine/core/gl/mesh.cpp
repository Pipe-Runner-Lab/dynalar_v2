#include "mesh.h"

#include <iostream>

Mesh::Mesh(std::vector<float> &vertices, std::vector<unsigned int> &indices,
           VertexBufferLayout &layout)
    : m_vertices(std::move(vertices)),
      m_indices(std::move(indices)),
      m_layout(std::move(layout)) {
    m_vertexArrayPtr = std::make_unique<VertexArray>();
    VertexBuffer vertexBuffer =
        VertexBuffer(m_vertices.data(), m_vertices.size() * sizeof(float));
    m_indexBufferPtr =
        std::make_unique<IndexBuffer>(m_indices.data(), m_indices.size());
    m_vertexArrayPtr->AddBuffer(vertexBuffer, m_layout);
}

Mesh::Mesh(Mesh &&other)
    : m_vertexArrayPtr(std::move(other.m_vertexArrayPtr)),
      m_indexBufferPtr(std::move(other.m_indexBufferPtr)) {
}

Mesh::~Mesh() {
}

void Mesh::Draw(Renderer &renderer) {
    renderer.Draw(*m_vertexArrayPtr, *m_indexBufferPtr);
}
