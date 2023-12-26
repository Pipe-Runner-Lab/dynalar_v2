#include "mesh.h"

#include <iostream>

Mesh::Mesh(std::vector<float> &vertices, std::vector<unsigned int> &indices,
           VertexBufferLayout &layout)
    : Mesh(vertices, indices, layout, nullptr) {
}

Mesh::Mesh(std::vector<float> &vertices, std::vector<unsigned int> &indices,
           VertexBufferLayout &layout, std::shared_ptr<Material> materialPtr)
    : m_materialPtr(materialPtr) {
    m_vertexArrayPtr = std::make_shared<VertexArray>();
    VertexBuffer vertexBuffer =
        VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
    m_indexBufferPtr =
        std::make_shared<IndexBuffer>(indices.data(), indices.size());
    m_vertexArrayPtr->AddBuffer(vertexBuffer, layout);
}

Mesh::Mesh(Mesh &&other)
    : m_vertexArrayPtr(std::move(other.m_vertexArrayPtr)),
      m_indexBufferPtr(std::move(other.m_indexBufferPtr)),
      m_materialPtr(std::move(other.m_materialPtr)) {
}

Mesh::~Mesh() {
}

void Mesh::Draw(Renderer &renderer, Shader &shader) {
    if (m_materialPtr)
        m_materialPtr->Bind(shader);
    renderer.Draw(*m_vertexArrayPtr, *m_indexBufferPtr);
    if (m_materialPtr)
        m_materialPtr->Unbind(shader);
}
