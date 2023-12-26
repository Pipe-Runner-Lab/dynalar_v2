#include "scene.h"

Hello3DWorldScene::Hello3DWorldScene(RenderContext &renderContext)
    : BaseScene(renderContext, "Hello 3D World"),
      m_shader("assets/shaders/vertex/simple_3d.vert",
               "assets/shaders/fragment/simple_3d.frag") {
    AddModel(Plane("Ground Plane", {0, 0, 0}, {-90, 0, 0}, {10, 10, 1},
                   {0.0, 1.0, 0.0, 1.0}));
    AddModel(Cube("Cube 1", {0, 1.01, 0}, {0, 0, 0}, {2, 2, 2},
                  {1.0, 0.0, 0.0, 1.0}));

    std::vector<float> vertices = {
        // 0 - +y
        -0.5f, 0.5f, 0.5f,  // xyz
        0.0f, 1.0f, 0.0f,   // normal
        1.0f, 0.0f,         // uv
        // 0 - +z
        -0.5f, 0.5f, 0.5f,  // xyz
        0.0f, 0.0f, 1.0f,   // normal
        1.0f, 0.0f,         // uv
        // 0 - -x
        -0.5f, 0.5f, 0.5f,  // xyz
        -1.0f, 0.0f, 0.0f,  // normal
        1.0f, 0.0f,         // uv
        // 1 - +y
        0.5f, 0.5f, 0.5f,  // xyz
        0.0f, 1.0f, 0.0f,  // normal
        1.0f, 0.0f,        // uv
        // 1 - +z
        0.5f, 0.5f, 0.5f,  // xyz
        0.0f, 0.0f, 1.0f,  // normal
        1.0f, 0.0f,        // uv
        // 1 - +x
        0.5f, 0.5f, 0.5f,  // xyz
        1.0f, 0.0f, 0.0f,  // normal
        1.0f, 0.0f,        // uv
        // 2 - -y
        0.5f, -0.5f, 0.5f,  // xyz
        0.0f, -1.0f, 0.0f,  // normal
        1.0f, 0.0f,         // uv
        // 2 - +z
        0.5f, -0.5f, 0.5f,  // xyz
        0.0f, 0.0f, 1.0f,   // normal
        1.0f, 0.0f,         // uv
        // 2 - +x
        0.5f, -0.5f, 0.5f,  // xyz
        1.0f, 0.0f, 0.0f,   // normal
        1.0f, 0.0f,         // uv
        // 3 - -y
        -0.5f, -0.5f, 0.5f,  // xyz
        0.0f, -1.0f, 0.0f,   // normal
        1.0f, 0.0f,          // uv
        // 3 - +z
        -0.5f, -0.5f, 0.5f,  // xyz
        0.0f, 0.0f, 1.0f,    // normal
        1.0f, 0.0f,          // uv
        // 3 - -x
        -0.5f, -0.5f, 0.5f,  // xyz
        -1.0f, 0.0f, 0.0f,   // normal
        1.0f, 0.0f,          // uv
        // 4 - +y
        0.5f, 0.5f, -0.5f,  // xyz
        0.0f, 1.0f, 0.0f,   // normal
        1.0f, 0.0f,         // uv
        // 4 - -z
        0.5f, 0.5f, -0.5f,  // xyz
        0.0f, 0.0f, -1.0f,  // normal
        1.0f, 0.0f,         // uv
        // 4 - +x
        0.5f, 0.5f, -0.5f,  // xyz
        1.0f, 0.0f, 0.0f,   // normal
        1.0f, 0.0f,         // uv
        // 5 - +y
        -0.5f, 0.5f, -0.5f,  // xyz
        0.0f, 1.0f, 0.0f,    // normal
        1.0f, 0.0f,          // uv
        // 5 - -z
        -0.5f, 0.5f, -0.5f,  // xyz
        0.0f, 0.0f, -1.0f,   // normal
        1.0f, 0.0f,          // uv
        // 5 - -x
        -0.5f, 0.5f, -0.5f,  // xyz
        -1.0f, 0.0f, 0.0f,   // normal
        1.0f, 0.0f,          // uv
        // 6 - -y
        -0.5f, -0.5f, -0.5f,  // xyz
        0.0f, -1.0f, 0.0f,    // normal
        1.0f, 0.0f,           // uv
        // 6 - -z
        -0.5f, -0.5f, -0.5f,  // xyz
        0.0f, 0.0f, -1.0f,    // normal
        1.0f, 0.0f,           // uv
        // 6 - -x
        -0.5f, -0.5f, -0.5f,  // xyz
        -1.0f, 0.0f, 0.0f,    // normal
        1.0f, 0.0f,           // uv
        // 7 - -y
        0.5f, -0.5f, -0.5f,  // xyz
        0.0f, -1.0f, 0.0f,   // normal
        1.0f, 0.0f,          // uv
        // 7 - -z
        0.5f, -0.5f, -0.5f,  // xyz
        0.0f, 0.0f, -1.0f,   // normal
        1.0f, 0.0f,          // uv
        // 7 - +x
        0.5f, -0.5f, -0.5f,  // xyz
        1.0f, 0.0f, 0.0f,    // normal
        1.0f, 0.0f,          // uv
    };
    std::vector<unsigned int> indices = {
        1,  7,  4,  7,  1,  10,  // Face 1 +z
        16, 13, 22, 22, 19, 16,  // Face 2 -z
        5,  8,  14, 8,  23, 14,  // Face 3 +x
        11, 17, 20, 11, 2,  17,  // Face 4 -x
        0,  3,  12, 12, 15, 0,   // Face 5 +y
        6,  9,  21, 21, 9,  18,  // Face 6 -y
    };
    VertexBufferLayout layout;
    layout.Push<float>(3);  // xyz
    layout.Push<float>(3);  // normal
    layout.Push<float>(2);  // uv
    std::vector<Mesh> meshGroup;
    std::shared_ptr<MeshBasicMaterial> material =
        std::make_shared<MeshBasicMaterial>();
    material->albedo = {0.0, 0.0, 1.0, 1.0};
    meshGroup.push_back(Mesh(vertices, indices, layout, material));
    AddModel(Model("Cube 2", meshGroup, {-2.5, 1.01, 0}, {0, 0, 0}, {2, 2, 2}));

    // set up camera
    AddCamera(Camera(
        glm::vec3(0, 5, 10), -30, -90,
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
    glm::mat4 vpMatrix =
        activeCamera.GetProjectionMatrix() * activeCamera.GetViewMatrix();

    for (auto &model : m_models) {
        model.Draw(*m_renderContext.rendererPtr, m_shader, vpMatrix);
    }
    m_shader.Unbind();
}

void Hello3DWorldScene::OnGUIRender() {
    BaseScene::OnGUIRender();
}
