#include "model.h"

Model::Model(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : tx(position.x),
      ty(position.y),
      tz(position.z),
      rx(rotation.x),
      ry(rotation.y),
      rz(rotation.z),
      sx(scale.x),
      sy(scale.y),
      sz(scale.z) {
    InitializeModelMatrix(position, rotation, scale);
}

Model::Model(std::vector<Mesh> &meshGroup, glm::vec3 position,
             glm::vec3 rotation, glm::vec3 scale)
    : m_meshes(meshGroup),
      tx(position.x),
      ty(position.y),
      tz(position.z),
      rx(rotation.x),
      ry(rotation.y),
      rz(rotation.z),
      sx(scale.x),
      sy(scale.y),
      sz(scale.z) {
    InitializeModelMatrix(position, rotation, scale);
}

Model::~Model() {
}

void Model::Draw(Renderer &renderer) {
    for (auto &mesh : m_meshes)
        mesh.Draw(renderer);
}

void Model::InitializeModelMatrix(glm::vec3 &position, glm::vec3 &rotation,
                                  glm::vec3 &scale) {
    Translate(position);
    Rotate(rotation);
    Scale(scale);

    m_initialModelMatrix = m_modelMatrix;
    m_initialModelMatrixInverse = glm::inverse(m_initialModelMatrix);
}
