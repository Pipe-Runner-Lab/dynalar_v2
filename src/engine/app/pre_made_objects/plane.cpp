#include "plane.h"

Plane::Plane()
{
  std::vector<float> vertices = {
      // 0
      10.0f, 0.0f, 10.0f,     // xyz
      0.0f, 0.0f, 1.0f,       // normal
      1.0f, 0.0f,             // uv
      1.0f, 1.0f, 1.0f, 1.0f, // color
      // 1
      10.0f, 0.0f, -10.0f,    // xyz
      0.0f, 0.0f, 1.0f,       // normal
      1.0f, 1.0f,             // uv
      1.0f, 1.0f, 1.0f, 1.0f, // color
      // 2
      -10.0f, 0.0f, -10.0f,   // xyz
      0.0f, 0.0f, 1.0f,       // normal
      0.0f, 1.0f,             // uv
      1.0f, 1.0f, 1.0f, 1.0f, // color
      // 3
      -10.0f, 0.0f, 10.0f,   // xyz
      0.0f, 0.0f, 1.0f,      // normal
      0.0f, 0.0f,            // uv
      1.0f, 1.0f, 1.0f, 1.0f // color
  };
  std::vector<unsigned int> indices = {
      0, 1, 2,
      2, 3, 0};
  VertexBufferLayout layout;
  layout.Push<float>(3); // xyz
  layout.Push<float>(3); // normal
  layout.Push<float>(2); // uv
  layout.Push<float>(4); // color
  m_meshes.push_back(Mesh(vertices, indices, layout));
}