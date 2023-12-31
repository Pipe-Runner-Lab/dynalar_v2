#include "scene.h"

HelloLightsScene::HelloLightsScene(RenderContext &renderContext)
    : BaseScene(renderContext, "Hello Lights"),
      m_shader("assets/shaders/vertex/light_3d.vert",
               "assets/shaders/fragment/light_3d.frag") {
    // AddModel(Plane("Ground Plane", {0, 0, 0}, {-90, 0, 0}, {10, 10, 1},
    //                {0.4, 0.3, 0.15, 1.0}));
    // AddModel(Plane("Left Wall", {-5, 5, 0}, {0, 90, 0}, {10, 10, 1},
    //                {0.27, 0.32, 0.33, 1.0}));
    // AddModel(Plane("Right Wall", {5, 5, 0}, {0, -90, 0}, {10, 10, 1},
    //                {0.27, 0.32, 0.33, 1.0}));
    // AddModel(Plane("Back Wall", {0, 5, -5}, {0, 0, 0}, {10, 10, 1},
    //                {0.27, 0.32, 0.33, 1.0}));
    // AddModel(Cube("Red Cube", {0, 1.01, 0}, {0, 0, 0}, {2, 2, 2},
    //               {1.0, 0.0, 0.0, 1.0}));
    // AddModel(Cube("Blue Cube", {-2.5, 1.01, 0}, {0, 0, 0}, {2, 2, 2},
    //               {0.0, 0.0, 1.0, 1.0}));
    // AddModel(Cube("Green Cube", {2.5, 1.01, 0}, {0, 0, 0}, {2, 2, 2},
    //               {0.0, 1.0, 0.0, 1.0}));
    AddModel(Model("Bag", "assets/models/backpack/backpack.obj", {0, 0, 0},
                   {0, 0, 0}, {1, 1, 1}));

    // set up camera
    AddCamera(Camera(
        glm::vec3(0, 8, 8), -30, -90,
        glm::perspective(glm::radians(45.0f),
                         renderContext.windowManagerPtr->GetAspectRatio(), 0.1f,
                         100.0f)));
    AddCamera(Camera(glm::vec3(0, 0, 10), 0, -90,
                     glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f)));

    // set up lights
    AddLight(std::make_unique<AmbientLight>(glm::vec3(1, 1, 1), 0.3f));
    AddLight(std::make_unique<PointLight>("Point Light 1",
                                          glm::vec3(0.68, 0.75, 0.55), 0.7f,
                                          glm::vec3(0, 5, 0)));
}

void HelloLightsScene::OnUpdate() {
    BaseScene::OnUpdate();

    // glm::vec3 currentRot = m_modelPtr->GetRotation();
    // if (m_modelPtr->GetPosition().z < 20) {
    //     m_modelPtr->Rotate({0.5f, 0, 0}).Translate({0, 0, 0.01f});
    // }

    // m_models[0].SetPosition({0, 0, -10}).SetOrientation({45, 0, 0});
}

void HelloLightsScene::OnRender() {
    m_shader.Bind();

    Camera &activeCamera = GetActiveCamera();
    glm::mat4 vpMatrix =
        activeCamera.GetProjectionMatrix() * activeCamera.GetViewMatrix();
    m_shader.SetUniform3f("u_viewPos", activeCamera.GetPosition().x,
                          activeCamera.GetPosition().y,
                          activeCamera.GetPosition().z);

    m_lightsContainer.Bind(m_shader);
    for (auto &lightPtr : m_lightsContainer.m_lightPtrs) {
        lightPtr->Draw(*m_renderContext.rendererPtr, m_shader, vpMatrix);
    }

    for (auto &model : m_models) {
        model.Draw(*m_renderContext.rendererPtr, m_shader, vpMatrix);
    }

    m_shader.Unbind();
}

void HelloLightsScene::OnGUIRender() {
    BaseScene::OnGUIRender();
}