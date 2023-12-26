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
    std::shared_ptr<VertexArray> m_vertexArrayPtr = nullptr;
    std::shared_ptr<IndexBuffer> m_indexBufferPtr = nullptr;
    std::shared_ptr<Material> m_materialPtr;

public:
    // TODO: This leads to dangling references. Fix it.
    Mesh(std::vector<float> &vertices, std::vector<unsigned int> &indices,
         VertexBufferLayout &layout);
    Mesh(std::vector<float> &vertices, std::vector<unsigned int> &indices,
         VertexBufferLayout &layout, std::shared_ptr<Material> material);

    Mesh(const Mesh &other) = delete;

    Mesh(Mesh &&other);

    ~Mesh();
    void Draw(Renderer &renderer, Shader &shader);

    inline std::shared_ptr<Material> GetMaterialPtr() {
        return m_materialPtr;
    }
};