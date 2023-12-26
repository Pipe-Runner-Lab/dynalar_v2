#include "cube.h"

Cube::Cube(std::string title, glm::vec3 position, glm::vec3 rotation,
           glm::vec3 scale, glm::vec4 color)
    : Model(title, position, rotation, scale) {
    std::vector<float> vertices = {
        // 0 - +y
        -0.5f, 0.5f, 0.5f,  // xyz
        0.0f, 1.0f, 0.0f,   // normal
        1.0f, 0.0f,         // uv
        // 0 - +z
        -0.5f, 0.5f, 0.5f,  // xyz
        0.0f, 0.0f, 1.0f,   // normal
        1.0f, 0.0f,         // uv
        // 0 - -x
        -0.5f, 0.5f, 0.5f,  // xyz
        -1.0f, 0.0f, 0.0f,  // normal
        1.0f, 0.0f,         // uv

        // 1 - +y
        0.5f, 0.5f, 0.5f,  // xyz
        0.0f, 1.0f, 0.0f,  // normal
        1.0f, 0.0f,        // uv
        // 1 - +z
        0.5f, 0.5f, 0.5f,  // xyz
        0.0f, 0.0f, 1.0f,  // normal
        1.0f, 0.0f,        // uv
        // 1 - +x
        0.5f, 0.5f, 0.5f,  // xyz
        1.0f, 0.0f, 0.0f,  // normal
        1.0f, 0.0f,        // uv

        // 2 - -y
        0.5f, -0.5f, 0.5f,  // xyz
        0.0f, -1.0f, 0.0f,  // normal
        1.0f, 0.0f,         // uv
        // 2 - +z
        0.5f, -0.5f, 0.5f,  // xyz
        0.0f, 0.0f, 1.0f,   // normal
        1.0f, 0.0f,         // uv
        // 2 - +x
        0.5f, -0.5f, 0.5f,  // xyz
        1.0f, 0.0f, 0.0f,   // normal
        1.0f, 0.0f,         // uv

        // 3 - -y
        -0.5f, -0.5f, 0.5f,  // xyz
        0.0f, -1.0f, 0.0f,   // normal
        1.0f, 0.0f,          // uv
        // 3 - +z
        -0.5f, -0.5f, 0.5f,  // xyz
        0.0f, 0.0f, 1.0f,    // normal
        1.0f, 0.0f,          // uv
        // 3 - -x
        -0.5f, -0.5f, 0.5f,  // xyz
        -1.0f, 0.0f, 0.0f,   // normal
        1.0f, 0.0f,          // uv

        // 4 - +y
        0.5f, 0.5f, -0.5f,  // xyz
        0.0f, 1.0f, 0.0f,   // normal
        1.0f, 0.0f,         // uv
        // 4 - -z
        0.5f, 0.5f, -0.5f,  // xyz
        0.0f, 0.0f, -1.0f,  // normal
        1.0f, 0.0f,         // uv
        // 4 - +x
        0.5f, 0.5f, -0.5f,  // xyz
        1.0f, 0.0f, 0.0f,   // normal
        1.0f, 0.0f,         // uv

        // 5 - +y
        -0.5f, 0.5f, -0.5f,  // xyz
        0.0f, 1.0f, 0.0f,    // normal
        1.0f, 0.0f,          // uv
        // 5 - -z
        -0.5f, 0.5f, -0.5f,  // xyz
        0.0f, 0.0f, -1.0f,   // normal
        1.0f, 0.0f,          // uv
        // 5 - -x
        -0.5f, 0.5f, -0.5f,  // xyz
        -1.0f, 0.0f, 0.0f,   // normal
        1.0f, 0.0f,          // uv

        // 6 - -y
        -0.5f, -0.5f, -0.5f,  // xyz
        0.0f, -1.0f, 0.0f,    // normal
        1.0f, 0.0f,           // uv
        // 6 - -z
        -0.5f, -0.5f, -0.5f,  // xyz
        0.0f, 0.0f, -1.0f,    // normal
        1.0f, 0.0f,           // uv
        // 6 - -x
        -0.5f, -0.5f, -0.5f,  // xyz
        -1.0f, 0.0f, 0.0f,    // normal
        1.0f, 0.0f,           // uv

        // 7 - -y
        0.5f, -0.5f, -0.5f,  // xyz
        0.0f, -1.0f, 0.0f,   // normal
        1.0f, 0.0f,          // uv
        // 7 - -z
        0.5f, -0.5f, -0.5f,  // xyz
        0.0f, 0.0f, -1.0f,   // normal
        1.0f, 0.0f,          // uv
        // 7 - +x
        0.5f, -0.5f, -0.5f,  // xyz
        1.0f, 0.0f, 0.0f,    // normal
        1.0f, 0.0f,          // uv
    };
    std::vector<unsigned int> indices = {
        1,  7,  4,  7,  1,  10,  // Face 1 +z
        16, 13, 22, 22, 19, 16,  // Face 2 -z
        5,  8,  14, 8,  23, 14,  // Face 3 +x
        11, 17, 20, 11, 2,  17,  // Face 4 -x
        0,  3,  12, 12, 15, 0,   // Face 5 +y
        6,  9,  21, 21, 9,  18,  // Face 6 -y
    };
    VertexBufferLayout layout;
    layout.Push<float>(3);  // xyz
    layout.Push<float>(3);  // normal
    layout.Push<float>(2);  // uv

    // material
    std::shared_ptr<MeshBasicMaterial> material =
        std::make_shared<MeshBasicMaterial>();
    material->albedo = color;

    m_meshesPtr->push_back(Mesh(vertices, indices, layout, material));
}