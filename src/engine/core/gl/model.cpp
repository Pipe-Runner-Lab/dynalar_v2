#include "model.h"

Model::Model(std::vector<Mesh> &meshGroup) : m_meshes(meshGroup), m_modelMatrix(glm::mat4(1.0f))
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
