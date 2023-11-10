#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "mesh.h"
#include "renderer.h"

class Model
{
private:
  std::vector<Mesh> m_meshes;
  glm::mat4 m_modelMatrix;

public:
  Model(std::vector<Mesh> &meshGroup);
  ~Model();
  void Draw(Renderer &renderer);

  inline glm::mat4 &GetModelMatrix() { return m_modelMatrix; }
};