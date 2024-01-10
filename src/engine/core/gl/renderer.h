#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "../utils/error.h"
#include "index_buffer.h"
#include "vertex_array.h"

class Renderer {
public:
    Renderer();

    void SetViewport(int x, int y, int width, int height);

    void Clear(GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    void Clear(float r, float g, float b, float a);
    void Draw(VertexArray &vertexArray, IndexBuffer &indexBuffer);

    // TODO: return by value is not efficient
    static glm::mat4 ComputeMVPMatrix(glm::mat4 &modelMatrix, glm::mat4 &viewMatrix,
                                      glm::mat4 &projectionMatrix);

    static glm::mat4 ComputeMVPMatrix(glm::mat4 &vpMatrix, glm::mat4 &modelMatrix);
};