#include "scene.h"

Hello3DWorldScene::Hello3DWorldScene(RenderContext &renderContext) : BaseScene(renderContext),
                                                                     m_shader("assets/shaders/vertex/clip_space.vert", "assets/shaders/fragment/simple_interpolated_color.frag")
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

  // set up camera
  AddCamera(Camera(
      glm::vec3(0, 0, 0),
      glm::vec3(0, 1.0f, 0),
      0,
      -90,
      glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f)));

  // set up editor callback
  m_objectPropertiesEditorPtr = std::make_shared<ObjectPropertiesEditor>(ObjectPropertiesEditor(
      [&]()
      {
        ImGui::Text("Hello 3D World!");
      }));
}

void Hello3DWorldScene::OnRender()
{
  m_shader.Bind();

  Camera &activeCamera = GetActiveCamera();

  glm::mat4 mvpMatrix = Renderer::ComputeMVPMatrix(
      m_modelPtr->GetModelMatrix(),
      activeCamera.GetViewMatrix(),
      activeCamera.GetProjectionMatrix());

  m_modelPtr->Draw(*m_renderContext.rendererPtr);
  m_shader.Unbind();
}

void Hello3DWorldScene::OnGUIRender()
{
  BaseScene::OnGUIRender(m_objectPropertiesEditorPtr);
}
