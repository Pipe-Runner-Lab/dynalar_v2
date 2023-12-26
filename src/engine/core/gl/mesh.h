#pragma once

#include <memory>
#include <vector>

#include "glm/gtc/matrix_transform.hpp"
#include "index_buffer.h"
#include "material.h"
#include "renderer.h"
#include "shader.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

// Note: Copying a mesh is not allowed. This is because mesh automatically
// uploads data to the GPU
class Mesh {
private:
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    VertexBufferLayout m_layout;  // TODO: Not used anywhere, remove?
    std::unique_ptr<VertexArray> m_vertexArrayPtr = nullptr;
    std::unique_ptr<IndexBuffer> m_indexBufferPtr = nullptr;

public:
    // TODO: This leads to dangling references. Fix it.
    Mesh(std::vector<float> &vertices, std::vector<unsigned int> &indices,
         VertexBufferLayout &layout);

    Mesh(const Mesh &other) = delete;

    Mesh(Mesh &&other);

    ~Mesh();
    void Draw(Renderer &renderer);
};