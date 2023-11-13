#pragma once

#include <GL/glew.h>

#include <stdexcept>
#include <vector>

#include "../utils/error.h"

struct VertexBufferElement {
    GLuint count;
    GLuint type;
    GLboolean normalized;

    static unsigned int GetSize(GLuint type);
};

class VertexBufferLayout {
private:
    std::vector<VertexBufferElement> m_VertexBufferElements;
    GLuint m_stride;

public:
    VertexBufferLayout();

    /*
      Each push is done for one type of item
      push(3) 3 for vertex
      push(3) 3 for color
      push(3) 3 for normal vector

      Thus one entine set if of 9 items, this is why stride is updated with +=
    */
    template <typename T>
    void Push(GLuint count);

    inline const std::vector<VertexBufferElement> &GetElements() const {
        return m_VertexBufferElements;
    };

    /**
     * @brief Returns the stride for the data layout
     * @return layout in bytes
     */
    inline GLuint GetStride() const {
        return m_stride;
    };
};
