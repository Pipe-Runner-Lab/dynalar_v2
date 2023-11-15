#include "scene.h"

Hello3DWorldScene::Hello3DWorldScene(RenderContext &renderContext)
    : BaseScene(renderContext),
      m_shader("assets/shaders/vertex/simple_3d.vert",
               "assets/shaders/fragment/simple_3d.frag") {
    m_modelPtr = std::make_unique<Model>(Plane(5, {0, -1.5f, 5}, {-45, 0, 0}));

    // set up camera
    AddCamera(Camera(
        glm::vec3(0, 0, 10), 0, -90,
        glm::perspective(glm::radians(45.0f),
                         renderContext.windowManagerPtr->GetAspectRatio(), 0.1f,
                         100.0f)));
    AddCamera(Camera(glm::vec3(0, 0, 10), 0, -90,
                     glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f)));

    // set up editor callback
    m_objectPropertiesEditorPtr = std::make_shared<ObjectPropertiesEditor>(
        ObjectPropertiesEditor([&]() { ImGui::Text("Hello 3D World!"); }));
}

void Hello3DWorldScene::OnUpdate() {
    BaseScene::OnUpdate();

    glm::vec3 currentRot = m_modelPtr->GetRotation();
    m_modelPtr->Rotate({currentRot.x, currentRot.y, currentRot.z});
}

void Hello3DWorldScene::OnRender() {
    m_shader.Bind();

    Camera &activeCamera = GetActiveCamera();

    glm::mat4 mvpMatrix = Renderer::ComputeMVPMatrix(
        m_modelPtr->GetModelMatrix(), activeCamera.GetViewMatrix(),
        activeCamera.GetProjectionMatrix());

    m_shader.SetUniformMatrix4f("u_mvp", mvpMatrix);
    m_shader.SetUniformBool("u_shouldUseTexture", false);

    m_modelPtr->Draw(*m_renderContext.rendererPtr);
    m_shader.Unbind();
}

void Hello3DWorldScene::OnGUIRender() {
    BaseScene::OnGUIRender(m_objectPropertiesEditorPtr);
}
