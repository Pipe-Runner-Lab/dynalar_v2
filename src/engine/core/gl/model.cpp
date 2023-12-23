#include "model.h"

Model::Model(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : m_position(position), m_rotation(rotation), m_scale(scale) {
    InitializeModelMatrix(position, rotation, scale);
}

Model::Model(std::vector<Mesh> &meshGroup, glm::vec3 position,
             glm::vec3 rotation, glm::vec3 scale)
    : m_meshesPtr(std::make_shared<std::vector<Mesh>>(meshGroup)),
      m_position(position),
      m_rotation(rotation),
      m_scale(scale) {
    InitializeModelMatrix(position, rotation, scale);
}

Model::~Model() {
}

void Model::Draw(Renderer &renderer) {
    for (auto &mesh : *m_meshesPtr)
        mesh.Draw(renderer);
}

void Model::InitializeModelMatrix(glm::vec3 &position, glm::vec3 &rotation,
                                  glm::vec3 &scale, bool forceReset) {
    if (forceReset) {
        m_position = glm::vec3(0.0f);
        m_rotation = glm::vec3(0.0f);
        m_scale = glm::vec3(1.0f);

        m_modelMatrix = glm::mat4(1.0f);
        m_initialModelMatrix = glm::mat4(1.0f);
        m_initialModelMatrixInverse = glm::mat4(1.0f);
    }

    Translate(position);
    Rotate(rotation);
    Scale(scale);

    m_initialModelMatrix = m_modelMatrix;
    m_initialModelMatrixInverse = glm::inverse(m_initialModelMatrix);
}
