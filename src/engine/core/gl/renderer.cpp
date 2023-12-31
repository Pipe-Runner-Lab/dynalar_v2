#include "renderer.h"

Renderer::Renderer() {
    /* Enable depth test */
    GL_CALL(glEnable(GL_DEPTH_TEST));

    /* Enable blending, set blending function and equation */
    GL_CALL(glEnable(GL_BLEND));
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_CALL(glBlendEquation(GL_FUNC_ADD));

    /* Enable face culling */
    // https://learnopengl.com/Advanced-OpenGL/Face-culling#:~:text=By%20default%2C%20triangles%20defined%20with%20counter%2Dclockwise%20vertices%20are%20processed%20as%20front%2Dfacing%20triangles.
    GL_CALL(glEnable(GL_CULL_FACE));
    GL_CALL(glCullFace(GL_BACK));
    GL_CALL(glFrontFace(GL_CCW));

    std::string glVersion = (char *)glGetString(GL_VERSION);
    fmt::print("Renderer using OpenGL version: {}", glVersion);
}

void Renderer::Clear() {
    GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Clear(float r, float g, float b, float a) {
    GL_CALL(glClearColor(r, g, b, a));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(VertexArray &vertexArray, IndexBuffer &indexBuffer) {
    vertexArray.Bind();
    indexBuffer.Bind();
    GL_CALL(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(),
                           GL_UNSIGNED_INT, nullptr));
    vertexArray.Unbind();
    indexBuffer.Unbind();
}

glm::mat4 Renderer::ComputeMVPMatrix(glm::mat4 &modelMatrix,
                                     glm::mat4 &viewMatrix,
                                     glm::mat4 &projectionMatrix) {
    return projectionMatrix * viewMatrix * modelMatrix;
}

glm::mat4 Renderer::ComputeMVPMatrix(glm::mat4 &vpMatrix,
                                     glm::mat4 &modelMatrix) {
    return vpMatrix * modelMatrix;
}
