#include "scene.h"

Hello3DWorldScene::Hello3DWorldScene(RenderContext &renderContext)
    : BaseScene(renderContext, "Hello 3D World"),
      m_shader("assets/shaders/vertex/simple_3d.vert",
               "assets/shaders/fragment/simple_3d.frag") {
    AddModel(Plane(5, {0, 0, 0}, {-90, 0, 0}));
    AddModel(Cube(2, {0, 1.01, 0}, {0, 0, 0}));

    // set up camera
    AddCamera(Camera(
        glm::vec3(0, 0, 10), 0, -90,
        glm::perspective(glm::radians(45.0f),
                         renderContext.windowManagerPtr->GetAspectRatio(), 0.1f,
                         100.0f)));
    AddCamera(Camera(glm::vec3(0, 0, 10), 0, -90,
                     glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f)));
}

void Hello3DWorldScene::OnUpdate() {
    BaseScene::OnUpdate();

    // glm::vec3 currentRot = m_modelPtr->GetRotation();
    // if (m_modelPtr->GetPosition().z < 20) {
    //     m_modelPtr->Rotate({0.5f, 0, 0}).Translate({0, 0, 0.01f});
    // }

    // m_models[0].SetPosition({0, 0, -10}).SetOrientation({45, 0, 0});
}

void Hello3DWorldScene::OnRender() {
    m_shader.Bind();

    Camera &activeCamera = GetActiveCamera();

    for (auto &model : m_models) {
        glm::mat4 mvpMatrix = Renderer::ComputeMVPMatrix(
            model.GetModelMatrix(), activeCamera.GetViewMatrix(),
            activeCamera.GetProjectionMatrix());

        m_shader.SetUniformMatrix4f("u_mvp", mvpMatrix);
        m_shader.SetUniformBool("u_shouldUseTexture", false);
        model.Draw(*m_renderContext.rendererPtr);
    }
    m_shader.Unbind();
}

void Hello3DWorldScene::OnGUIRender() {
    BaseScene::OnGUIRender();
}
