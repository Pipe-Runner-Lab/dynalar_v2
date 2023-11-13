#pragma once

#include <GL/glew.h>

#include "../utils/error.h"
#include "./vertex_buffer.h"
#include "./vertex_buffer_layout.h"

class VertexArray {
private:
    GLuint m_arrayID;

public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);

    void Bind() const;
    void Unbind() const;
};
