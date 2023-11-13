#include "scene.h"

HelloTextureScene::HelloTextureScene(RenderContext &renderContext)
    : BaseScene(renderContext),
      m_shader("assets/shaders/vertex/simple_uv.vert",
               "assets/shaders/fragment/simple_texture.frag"),
      m_texture("assets/textures/default.png", false) {
    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f,  // 0 - xyz
        0.0f,  0.0f,         // 0 - uv
        0.5f,  -0.5f, 0.0f,  // 1 - xyz
        1.0f,  0.0f,         // 1 - uv
        0.0f,  0.5f,  0.0f,  // 2 - xyz
        0.5f,  0.5f          // 2 - uv
    };
    std::vector<unsigned int> indices = {0, 1, 2};
    VertexBufferLayout layout;
    layout.Push<float>(3);  // xyz
    layout.Push<float>(2);  // uv
    std::vector<Mesh> meshes = {Mesh(vertices, indices, layout)};

    m_modelPtr = std::make_unique<Model>(Model(meshes));

    m_texture.Bind();

    m_shader.Bind();
    m_shader.SetUniform1i("u_textureSampler", 0);
}

void HelloTextureScene::OnRender() {
    m_shader.Bind();
    m_modelPtr->Draw(*m_renderContext.rendererPtr);
    m_shader.Unbind();
}