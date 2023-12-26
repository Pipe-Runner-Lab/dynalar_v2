#include "plane.h"

Plane::Plane(std::string title, glm::vec3 position, glm::vec3 rotation,
             glm::vec3 scale, glm::vec4 color)
    : Model(title, position, rotation, scale) {
    std::vector<float> vertices = {
        // 0
        0.5f, 0.5f, 0.0f,  // xyz
        0.0f, 0.0f, 1.0f,  // normal
        1.0f, 0.0f,        // uv
        // 1
        0.5f, -0.5f, 0.0f,  // xyz
        0.0f, 0.0f, 1.0f,   // normal
        1.0f, 1.0f,         // uv
        // 2
        -0.5f, -0.5f, 0.0f,  // xyz
        0.0f, 0.0f, 1.0f,    // normal
        0.0f, 1.0f,          // uv
        // 3
        -0.5f, 0.5f, 0.0f,  // xyz
        0.0f, 0.0f, 1.0f,   // normal
        0.0f, 0.0f,         // uv
    };
    std::vector<unsigned int> indices = {0, 2, 1, 2, 0, 3};
    VertexBufferLayout layout;
    layout.Push<float>(3);  // xyz
    layout.Push<float>(3);  // normal
    layout.Push<float>(2);  // uv

    // material
    std::shared_ptr<MeshBasicMaterial> materialPtr =
        std::make_shared<MeshBasicMaterial>();
    materialPtr->albedo = color;
    m_materialPtrs.push_back(materialPtr);

    m_meshesPtr->push_back(Mesh(vertices, indices, layout));
}