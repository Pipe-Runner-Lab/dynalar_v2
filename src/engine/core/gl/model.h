#pragma once

#include <vector>
#include "mesh.h"
#include "renderer.h"

class Model
{
private:
  std::vector<Mesh> m_meshes;

public:
  Model(std::vector<Mesh> &meshGroup);
  ~Model();
  void Draw(Renderer &renderer);
};