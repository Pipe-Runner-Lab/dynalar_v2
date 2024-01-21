#include "scene.h"

HelloLightsScene::HelloLightsScene(RenderContext &renderContext)
    : BaseScene(renderContext, "Hello Lights") {
    // set up shader
    // m_shaderPtrs.emplace_back("assets/shaders/vertex/light_3d.vert",
    //                           "assets/shaders/fragment/light_3d.frag");
    // m_shaderPtrs.emplace_back("assets/shaders/vertex/simple_shadow.vert",
    //                           "assets/shaders/fragment/simple_shadow.frag");

    AddShader(std::make_unique<Shader>("assets/shaders/vertex/light_3d.vert",
                                       "assets/shaders/fragment/light_3d.frag"));
    AddShader(std::make_unique<Shader>("assets/shaders/vertex/simple_shadow.vert",
                                       "assets/shaders/fragment/simple_shadow.frag"));

    AddModel(std::make_unique<Plane>("Ground Plane", glm::vec3(0, 0, 0), glm::vec3(-90, 0, 0),
                                     glm::vec3(10, 10, 1), glm::vec4(0.4, 0.3, 0.15, 1.0)));
    AddModel(std::make_unique<Plane>("Right Wall", glm::vec3(5, 5, 0), glm::vec3(0, -90, 0),
                                     glm::vec3(10, 10, 1), glm::vec4(0.27, 0.32, 0.33, 1.0)));
    AddModel(std::make_unique<Plane>("Back Wall", glm::vec3(0, 5, -5), glm::vec3(0, 0, 0),
                                     glm::vec3(10, 10, 1), glm::vec4(0.27, 0.32, 0.33, 1.0)));
    AddModel(std::make_unique<Cube>("Base", glm::vec3(0, 1.01, 0), glm::vec3(0, 0, 0),
                                    glm::vec3(4, 2, 4), glm::vec4(0.18, 0.102, 0.102, 1.0)));
    AddModel(std::make_unique<Model>("Helmet", "assets/models/flight_helmet/FlightHelmet.gltf",
                                     glm::vec3(0, 2, 0), glm::vec3(0, -90, 0), glm::vec3(4, 4, 4)));

    // axis debugging
    // float scale = 10;
    // AddModel(std::make_unique<Cube>("Origin", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),
    //                                 glm::vec3(0.1, 0.1, 0.1), glm::vec4(1, 1, 1, 1)));  // origin
    // AddModel(std::make_unique<Cube>("X Axis", glm::vec3(scale, 0, 0), glm::vec3(0, 0, 0),
    //                                 glm::vec3(scale / 2, 0.05, 0.05),
    //                                 glm::vec4(1, 0, 0, 1)));  // x axis (red)
    // AddModel(std::make_unique<Cube>("Y Axis", glm::vec3(0, scale, 0), glm::vec3(0, 0, 0),
    //                                 glm::vec3(0.05, scale / 2, 0.05),
    //                                 glm::vec4(0, 1, 0, 1)));  // y axis (green)
    // AddModel(std::make_unique<Cube>("Z Axis", glm::vec3(0, 0, scale), glm::vec3(0, 0, 0),
    //                                 glm::vec3(0.05, 0.05, scale / 2),
    //                                 glm::vec4(0, 0, 1, 1)));  // z axis (blue)

    // AddModel(Model("sponza", "assets/models/sponza/Sponza.gltf", {0, 0, 0}, {0, 0, 0}, {1, 1,
    // 1}));

    // set up camera
    AddCamera(std::make_unique<Camera>(
        glm::vec3(-4.6, 6.9, 5.7), -27.2, -47.7,
        glm::perspective(glm::radians(45.0f), renderContext.windowManagerPtr->GetAspectRatio(),
                         0.1f, 100.0f)));
    AddCamera(std::make_unique<Camera>(glm::vec3(0, 0, 10), 0, -90,
                                       glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f)));

    // set up lights
    AddLight(std::make_unique<AmbientLight>(glm::vec3(1, 1, 1), 0.034f));
    // AddLight(std::make_unique<PointLight>("Point Light 1", glm::vec3(1, 1, 1), 0.0f, 0.5f, 0.4f,
    //                                       glm::vec3(-2.6, 6.2, 2.9)));
    // AddLight(std::make_unique<SpotLight>("Spot Light 1", glm::vec3(0.93, 0.95, 0.45), 0.0f, 0.5f,
    //                                      0.4f, glm::vec3(0, 7, 0), glm::vec3(0, -1, 0), 15.5f,
    //                                      25.0f));
    AddLight(std::make_unique<DirectionalLight>("Directional Light 1", glm::vec3(1, 1, 1), 0.0f,
                                                0.5f, 0.4f, glm::vec3(1, -1, -1)));
    AddLight(std::make_unique<DirectionalLight>("Directional Light 2", glm::vec3(1, 0, 1), 0.0f,
                                                0.5f, 0.4f, glm::vec3(1, -1, -0.5)));
}

void HelloLightsScene::OnUpdate() {
    BaseScene::OnUpdate();

    // m_modelPtrs[3]->Rotate({0, 0.2f, 0});
    // m_modelPtrs[4]->Rotate({0, 0.2f, 0});
}

void HelloLightsScene::OnRender() {
    m_lightsManager.directionalShadowMapCount = 1;

    // 1. render shadow maps
    m_shaderPtrs[1]->Bind();
    m_lightsManager.GenerateShadowMaps(*m_renderContext.rendererPtr,
                                       *m_renderContext.windowManagerPtr, m_modelPtrs,
                                       *m_shaderPtrs[1]);
    m_shaderPtrs[1]->Unbind();

    // 2. render scene
    m_shaderPtrs[0]->Bind();

    // 2.1 set light uniforms
    m_lightsManager.Bind(*m_shaderPtrs[0]);

    // 2.2 activate shadow map slots
    m_lightsManager.ActivateShadowMaps(*m_shaderPtrs[0]);

    Camera &activeCamera = GetActiveCamera();
    glm::mat4 vpMatrix = activeCamera.GetProjectionMatrix() * activeCamera.GetViewMatrix();
    m_shaderPtrs[0]->SetUniform3f("u_viewPos", activeCamera.GetPosition().x,
                                  activeCamera.GetPosition().y, activeCamera.GetPosition().z);

    for (auto &modelPtr : m_modelPtrs) {
        modelPtr->Draw(*m_renderContext.rendererPtr, *m_shaderPtrs[0], vpMatrix,
                       m_lightsManager.directionalShadowMapCount);
    }

    // 3. render light models
    for (auto &lightPtr : m_lightsManager.lightPtrs) {
        lightPtr->Draw(*m_renderContext.rendererPtr, *m_shaderPtrs[0], vpMatrix);
    }

    m_shaderPtrs[0]->Unbind();
}
