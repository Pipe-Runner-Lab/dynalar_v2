#include "model.h"

Model::Model(std::vector<Mesh> &meshGroup) : m_meshes(meshGroup)
{
}

Model::~Model()
{
}

void Model::Draw(Renderer &renderer)
{
  for (auto &mesh : m_meshes)
    mesh.Draw(renderer);
}
