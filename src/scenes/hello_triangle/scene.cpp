#include "scene.h"

HelloTriangleScene::HelloTriangleScene(RenderContext &renderContext)
    : BaseScene(renderContext, "Hello Triangle"),
      m_shader("assets/shaders/vertex/clip_space.vert",
               "assets/shaders/fragment/simple_interpolated_color.frag") {
    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f,  // 0
        0.5f,  -0.5f, 0.0f,  // 1
        0.0f,  0.5f,  0.0f   // 2
    };
    std::vector<unsigned int> indices = {0, 1, 2};
    VertexBufferLayout layout;
    layout.Push<float>(3);
    std::vector<Mesh> meshes = {Mesh(vertices, indices, layout)};

    m_models.push_back(std::move(Model("Triangle", meshes)));
}

void HelloTriangleScene::OnRender() {
    m_shader.Bind();
    m_models[0].Draw(*m_renderContext.rendererPtr);
    m_shader.Unbind();
}