#include "mesh.h"

Mesh::Mesh(std::vector<float> &vertices, std::vector<unsigned int> &indices, VertexBufferLayout &layout)
{
  m_vertexArray = std::make_shared<VertexArray>();
  VertexBuffer vertexBuffer = VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
  m_indexBuffer = std::make_shared<IndexBuffer>(indices.data(), indices.size());
  m_vertexArray->AddBuffer(vertexBuffer, layout);
}

Mesh::~Mesh()
{
}

void Mesh::Draw(Renderer &renderer)
{
  renderer.Draw(*m_vertexArray, *m_indexBuffer);
}
