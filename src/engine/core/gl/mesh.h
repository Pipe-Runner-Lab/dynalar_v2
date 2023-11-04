#include "index_buffer.h"
#include "vertex_buffer.h"
#include "vertex_array.h"
#include "renderer.h"

class Mesh
{
private:
  VertexArray m_VertexArray;
  VertexBuffer m_VertexBuffer;
  IndexBuffer m_IndexBuffer;

public:
  Mesh();
  ~Mesh();
  void Draw(Renderer &renderer);
};