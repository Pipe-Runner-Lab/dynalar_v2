#include "plane.h"

Plane::Plane(float size, glm::vec3 position, glm::vec3 rotation,
             glm::vec3 scale)
    : Model(position, rotation, scale) {
    std::vector<float> vertices = {
        // 0
        0.5f * size, 0.5f * size, 0.0f,  // xyz
        0.0f, 0.0f, 1.0f,                // normal
        1.0f, 0.0f,                      // uv
        1.0f, 1.0f, 0.0f, 1.0f,          // color
        // 1
        0.5f * size, -0.5f * size, 0.0f,  // xyz
        0.0f, 0.0f, 1.0f,                 // normal
        1.0f, 1.0f,                       // uv
        1.0f, 1.0f, 0.0f, 1.0f,           // color
        // 2
        -0.5f * size, -0.5f * size, 0.0f,  // xyz
        0.0f, 0.0f, 1.0f,                  // normal
        0.0f, 1.0f,                        // uv
        0.0f, 1.0f, 1.0f, 1.0f,            // color
        // 3
        -0.5f * size, 0.5f * size, 0.0f,  // xyz
        0.0f, 0.0f, 1.0f,                 // normal
        0.0f, 0.0f,                       // uv
        0.0f, 1.0f, 1.0f, 1.0f            // color
    };
    std::vector<unsigned int> indices = {0, 2, 1, 2, 0, 3};
    VertexBufferLayout layout;
    layout.Push<float>(3);  // xyz
    layout.Push<float>(3);  // normal
    layout.Push<float>(2);  // uv
    layout.Push<float>(4);  // color
    m_meshes.push_back(Mesh(vertices, indices, layout));
}