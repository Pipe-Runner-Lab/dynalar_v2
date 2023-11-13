#pragma once

#include <GL/glew.h>

#include "../utils/error.h"

class IndexBuffer {
private:
    GLuint m_bufferID = 0, m_Count = 0;

public:
    IndexBuffer(const GLuint *data, GLuint count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline GLuint GetCount() const {
        return m_Count;
    }
};
