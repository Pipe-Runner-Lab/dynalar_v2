#pragma once

#include <memory>
#include <vector>

#include "index_buffer.h"
#include "renderer.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

class Mesh {
private:
    std::shared_ptr<VertexArray> m_vertexArray = nullptr;
    std::shared_ptr<IndexBuffer> m_indexBuffer = nullptr;

public:
    // TODO: This leads to dangling references. Fix it.
    Mesh(std::vector<float> &vertices, std::vector<unsigned int> &indices,
         VertexBufferLayout &layout);
    ~Mesh();
    void Draw(Renderer &renderer);
};