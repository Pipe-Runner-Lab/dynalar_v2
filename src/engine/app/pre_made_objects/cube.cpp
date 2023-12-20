#include "cube.h"

Cube::Cube(float size, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : Model(position, rotation, scale) {
    std::vector<float> vertices = {
        // 0 - +y
        -0.5f * size, 0.5f * size, -0.5f * size,  // xyz
        0.0f, 1.0f, 0.0f,                         // normal
        1.0f, 0.0f,                               // uv
        1.0f, 1.0f, 0.0f, 1.0f,                   // color
        // 0 - -z
        -0.5f * size, 0.5f * size, -0.5f * size,  // xyz
        0.0f, 0.0f, -1.0f,                        // normal
        1.0f, 0.0f,                               // uv
        1.0f, 1.0f, 0.0f, 1.0f,                   // color
        // 0 - -x
        -0.5f * size, 0.5f * size, -0.5f * size,  // xyz
        -1.0f, 0.0f, 0.0f,                        // normal
        1.0f, 0.0f,                               // uv
        1.0f, 1.0f, 0.0f, 1.0f,                   // color

        // 1 - +y
        0.5f * size, 0.5f * size, -0.5f * size,  // xyz
        0.0f, 1.0f, 0.0f,                        // normal
        1.0f, 0.0f,                              // uv
        1.0f, 1.0f, 0.0f, 1.0f,                  // color
        // 1 - -z
        0.5f * size, 0.5f * size, -0.5f * size,  // xyz
        0.0f, 0.0f, -1.0f,                       // normal
        1.0f, 0.0f,                              // uv
        1.0f, 1.0f, 0.0f, 1.0f,                  // color
        // 1 - +x
        0.5f * size, 0.5f * size, -0.5f * size,  // xyz
        1.0f, 0.0f, 0.0f,                        // normal
        1.0f, 0.0f,                              // uv
        1.0f, 1.0f, 0.0f, 1.0f,                  // color

        // 2 - -y
        0.5f * size, -0.5f * size, -0.5f * size,  // xyz
        0.0f, -1.0f, 0.0f,                        // normal
        1.0f, 0.0f,                               // uv
        1.0f, 1.0f, 0.0f, 1.0f,                   // color
        // 2 - -z
        0.5f * size, -0.5f * size, -0.5f * size,  // xyz
        0.0f, 0.0f, -1.0f,                        // normal
        1.0f, 0.0f,                               // uv
        1.0f, 1.0f, 0.0f, 1.0f,                   // color
        // 2 - +x
        0.5f * size, -0.5f * size, -0.5f * size,  // xyz
        1.0f, 0.0f, 0.0f,                         // normal
        1.0f, 0.0f,                               // uv
        1.0f, 1.0f, 0.0f, 1.0f,                   // color

        // 3 - -y
        -0.5f * size, -0.5f * size, -0.5f * size,  // xyz
        0.0f, -1.0f, 0.0f,                         // normal
        1.0f, 0.0f,                                // uv
        1.0f, 1.0f, 0.0f, 1.0f,                    // color
        // 3 - -z
        -0.5f * size, -0.5f * size, -0.5f * size,  // xyz
        0.0f, 0.0f, -1.0f,                         // normal
        1.0f, 0.0f,                                // uv
        1.0f, 1.0f, 0.0f, 1.0f,                    // color
        // 3 - -x
        -0.5f * size, -0.5f * size, -0.5f * size,  // xyz
        -1.0f, 0.0f, 0.0f,                         // normal
        1.0f, 0.0f,                                // uv
        1.0f, 1.0f, 0.0f, 1.0f,                    // color

        // 4 - +y
        0.5f * size, 0.5f * size, 0.5f * size,  // xyz
        0.0f, 1.0f, 0.0f,                       // normal
        1.0f, 0.0f,                             // uv
        1.0f, 1.0f, 0.0f, 1.0f,                 // color
        // 4 - +z
        0.5f * size, 0.5f * size, 0.5f * size,  // xyz
        0.0f, 0.0f, 1.0f,                       // normal
        1.0f, 0.0f,                             // uv
        1.0f, 1.0f, 0.0f, 1.0f,                 // color
        // 4 - +x
        0.5f * size, 0.5f * size, 0.5f * size,  // xyz
        1.0f, 0.0f, 0.0f,                       // normal
        1.0f, 0.0f,                             // uv
        1.0f, 1.0f, 0.0f, 1.0f,                 // color

        // 5 - +y
        -0.5f * size, 0.5f * size, 0.5f * size,  // xyz
        0.0f, 1.0f, 0.0f,                        // normal
        1.0f, 0.0f,                              // uv
        1.0f, 1.0f, 0.0f, 1.0f,                  // color
        // 5 - +z
        -0.5f * size, 0.5f * size, 0.5f * size,  // xyz
        0.0f, 0.0f, 1.0f,                        // normal
        1.0f, 0.0f,                              // uv
        1.0f, 1.0f, 0.0f, 1.0f,                  // color
        // 5 - -x
        -0.5f * size, 0.5f * size, 0.5f * size,  // xyz
        -1.0f, 0.0f, 0.0f,                       // normal
        1.0f, 0.0f,                              // uv
        1.0f, 1.0f, 0.0f, 1.0f,                  // color

        // 6 - -y
        -0.5f * size, -0.5f * size, 0.5f * size,  // xyz
        0.0f, -1.0f, 0.0f,                        // normal
        1.0f, 0.0f,                               // uv
        1.0f, 1.0f, 0.0f, 1.0f,                   // color
        // 6 - +z
        -0.5f * size, -0.5f * size, 0.5f * size,  // xyz
        0.0f, 0.0f, 1.0f,                         // normal
        1.0f, 0.0f,                               // uv
        1.0f, 1.0f, 0.0f, 1.0f,                   // color
        // 6 - -x
        -0.5f * size, -0.5f * size, 0.5f * size,  // xyz
        -1.0f, 0.0f, 0.0f,                        // normal
        1.0f, 0.0f,                               // uv
        1.0f, 1.0f, 0.0f, 1.0f,                   // color

        // 7 - -y
        0.5f * size, -0.5f * size, 0.5f * size,  // xyz
        0.0f, -1.0f, 0.0f,                       // normal
        1.0f, 0.0f,                              // uv
        1.0f, 1.0f, 0.0f, 1.0f,                  // color
        // 7 - +z
        0.5f * size, -0.5f * size, 0.5f * size,  // xyz
        0.0f, 0.0f, 1.0f,                        // normal
        1.0f, 0.0f,                              // uv
        1.0f, 1.0f, 0.0f, 1.0f,                  // color
        // 7 - +x
        0.5f * size, -0.5f * size, 0.5f * size,  // xyz
        1.0f, 0.0f, 0.0f,                        // normal
        1.0f, 0.0f,                              // uv
        1.0f, 1.0f, 0.0f, 1.0f,                  // color
    };
    std::vector<unsigned int> indices = {
        1,  4,  7,  7,  10, 1,   // Face 1
        22, 13, 16, 22, 16, 19,  // Face 2
        5,  14, 23, 23, 8,  5,   // Face 3
        20, 18, 2,  20, 2,  11,  // Face 4
        0,  15, 3,  15, 12, 3,   // Face 5
        6,  15, 18, 6,  18, 9,   // Face 6
    };
    VertexBufferLayout layout;
    layout.Push<float>(3);  // xyz
    layout.Push<float>(3);  // normal
    layout.Push<float>(2);  // uv
    layout.Push<float>(4);  // color
    m_meshes.push_back(Mesh(vertices, indices, layout));
}