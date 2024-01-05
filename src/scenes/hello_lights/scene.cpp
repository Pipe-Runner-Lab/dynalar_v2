#include "scene.h"

HelloLightsScene::HelloLightsScene(RenderContext &renderContext)
    : BaseScene(renderContext, "Hello Lights") {
    // set up shader
    m_shaders.emplace_back("assets/shaders/vertex/light_3d.vert",
                           "assets/shaders/fragment/light_3d.frag");

    AddModel(Plane("Ground Plane", {0, 0, 0}, {-90, 0, 0}, {10, 10, 1}, {0.4, 0.3, 0.15, 1.0}));
    AddModel(Plane("Right Wall", {5, 5, 0}, {0, -90, 0}, {10, 10, 1}, {0.27, 0.32, 0.33, 1.0}));
    AddModel(Plane("Back Wall", {0, 5, -5}, {0, 0, 0}, {10, 10, 1}, {0.27, 0.32, 0.33, 1.0}));
    AddModel(Cube("Base", {0, 1.01, 0}, {0, 0, 0}, {4, 2, 4}, {0.18, 0.102, 0.102, 1.0}));
    AddModel(Model("Helmet", "assets/models/flight_helmet/FlightHelmet.gltf", {0, 2, 0},
                   {0, -90, 0}, {4, 4, 4}));

    // AddModel(Model("sponza", "assets/models/sponza/Sponza.gltf", {0, 0, 0}, {0, 0, 0}, {1, 1,
    // 1}));

    // set up camera
    AddCamera(
        Camera(glm::vec3(-4.6, 6.9, 5.7), -27.2, -47.7,
               glm::perspective(glm::radians(45.0f),
                                renderContext.windowManagerPtr->GetAspectRatio(), 0.1f, 100.0f)));
    AddCamera(Camera(glm::vec3(0, 0, 10), 0, -90,
                     glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f)));

    // set up lights
    AddLight(std::make_unique<AmbientLight>(glm::vec3(1, 1, 1), 0.034f));
    // AddLight(std::make_unique<PointLight>("Point Light 1", glm::vec3(1, 1, 1), 0.0f, 0.5f, 0.4f,
    //                                       glm::vec3(-2.6, 6.2, 2.9)));
    // AddLight(std::make_unique<SpotLight>("Spot Light 1", glm::vec3(0.93, 0.95, 0.45), 0.0f, 0.5f,
    //                                      0.4f, glm::vec3(0, 7, 0), glm::vec3(0, -1, 0), 15.5f,
    //                                      25.0f));
    AddLight(std::make_unique<DirectionalLight>("Directional Light", glm::vec3(1, 1, 1), 0.0f, 0.5f,
                                                0.4f, glm::vec3(-1, -1, -1)));
}

void HelloLightsScene::OnUpdate() {
    BaseScene::OnUpdate();

    m_models[3].Rotate({0, 0.2f, 0});
    m_models[4].Rotate({0, 0.2f, 0});
}

void HelloLightsScene::OnRender() {
    m_shaders[0].Bind();

    Camera &activeCamera = GetActiveCamera();
    glm::mat4 vpMatrix = activeCamera.GetProjectionMatrix() * activeCamera.GetViewMatrix();
    m_shaders[0].SetUniform3f("u_viewPos", activeCamera.GetPosition().x,
                              activeCamera.GetPosition().y, activeCamera.GetPosition().z);

    for (auto &model : m_models) {
        model.Draw(*m_renderContext.rendererPtr, m_shaders[0], vpMatrix);
    }

    // change shader here if needed
    m_lightsContainer.Bind(m_shaders[0]);
    for (auto &lightPtr : m_lightsContainer.m_lightPtrs) {
        lightPtr->Draw(*m_renderContext.rendererPtr, m_shaders[0], vpMatrix);
    }

    m_shaders[0].Unbind();
}
