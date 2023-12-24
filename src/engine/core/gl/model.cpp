#include "model.h"

Model::Model(std::string title, glm::vec3 position, glm::vec3 rotation,
             glm::vec3 scale)
    : title(title), m_position(position), m_rotation(rotation), m_scale(scale) {
}

Model::Model(std::string title, std::vector<Mesh> &meshGroup,
             glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : title(title),
      m_meshesPtr(std::make_shared<std::vector<Mesh>>(meshGroup)),
      m_position(position),
      m_rotation(rotation),
      m_scale(scale) {
}

Model::~Model() {
}

void Model::Draw(Renderer &renderer) {
    for (auto &mesh : *m_meshesPtr)
        mesh.Draw(renderer);
}
