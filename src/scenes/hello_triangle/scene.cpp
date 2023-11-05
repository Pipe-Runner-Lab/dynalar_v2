#include "scene.h"

HelloTriangleScene::HelloTriangleScene(RenderContext &renderContext) : BaseScene(&renderContext), m_shader("assets/shaders/vertex/clip_space.vert", "assets/shaders/fragment/simple_interpolated_color.frag")
{
  std::vector<float> vertices = {
      -0.5f, -0.5f, 0.0f, // 0
      0.5f, -0.5f, 0.0f,  // 1
      0.0f, 0.5f, 0.0f    // 2
  };
  std::vector<unsigned int> indices = {
      0, 1, 2};
  VertexBufferLayout layout;
  layout.Push<float>(3);
  std::vector<Mesh> meshes = {
      Mesh(vertices, indices, layout)};

  m_modelPtr = std::make_unique<Model>(Model(meshes));
}

void HelloTriangleScene::OnRender()
{
  m_shader.Bind();
  m_modelPtr->Draw(*m_renderContextPtr->rendererPtr);
  m_shader.Unbind();
}