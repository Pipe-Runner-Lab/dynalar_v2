#pragma once

#include <vector>
#include "mesh.h"
#include "renderer.h"

class Model
{
private:
  std::vector<Mesh> m_meshes;

public:
  Model();
  ~Model();
  void Draw(Renderer &renderer);
};