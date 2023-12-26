#include "model.h"

Model::Model(std::string title, glm::vec3 position, glm::vec3 rotation,
             glm::vec3 scale)
    : title(title),
      m_position(position),
      m_rotation(rotation),
      m_scale(scale),
      m_meshesPtr(std::make_shared<std::vector<Mesh>>()) {
}

Model::Model(std::string title, std::vector<Mesh> &meshGroup,
             glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : title(title),
      m_meshesPtr(std::make_shared<std::vector<Mesh>>(std::move(meshGroup))),
      m_position(position),
      m_rotation(rotation),
      m_scale(scale) {
}

Model::Model(std::string title, std::vector<Mesh> &meshGroup,
             std::vector<std::shared_ptr<Material>> &materialPtrs,
             glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : title(title),
      m_meshesPtr(std::make_shared<std::vector<Mesh>>(std::move(meshGroup))),
      //   m_materialPtrs(std::move(materialPtrs)),
      m_position(position),
      m_rotation(rotation),
      m_scale(scale) {
    for (int idx = 0; idx < materialPtrs.size(); idx++) {
        m_materialPtrs.push_back(materialPtrs[idx]);
    }
}

Model::Model(std::string title, std::string path, glm::vec3 position,
             glm::vec3 rotation, glm::vec3 scale)
    : m_path(path), m_directory(path.substr(0, path.find_last_of('/'))) {
    Assimp::Importer importer;
    const aiScene *scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        throw std::runtime_error(
            fmt::format("Assimp error - {}.\n", importer.GetErrorString()));
    }

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshesPtr->push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat,
                                                 aiTextureType type,
                                                 std::string typeName) {
}

Model::Model(Model &&other)
    : title(std::move(other.title)),
      m_position(std::move(other.m_position)),
      m_rotation(std::move(other.m_rotation)),
      m_scale(std::move(other.m_scale)),
      m_meshesPtr(std::move(other.m_meshesPtr)),
      m_materialPtrs(std::move(other.m_materialPtrs)){};

Model::~Model() {
}

void Model::Draw(Renderer &renderer, Shader &shader, glm::mat4 vpMatrix) {
    glm::mat4 modelMatrix = GetModelMatrix();

    shader.SetUniformMatrix4f(
        "u_mvp", Renderer::ComputeMVPMatrix(vpMatrix, modelMatrix));
    shader.SetUniformMatrix4f("u_model", modelMatrix);
    shader.SetUniformBool("u_shouldUseTexture", false);
    shader.SetUniformBool("u_debugNormals", debugNormals);

    Draw(renderer, shader);
}

void Model::Draw(Renderer &renderer, Shader &shader) {
    bool hasMaterial = m_materialPtrs.size() > 0;

    if (hasMaterial && m_materialPtrs.size() != m_meshesPtr->size()) {
        throw std::runtime_error(
            fmt::format("Material count ({}) does not match mesh count ({}).\n",
                        m_materialPtrs.size(), m_meshesPtr->size()));
    }

    for (int meshIdx = 0; meshIdx < m_meshesPtr->size(); meshIdx++) {
        if (hasMaterial) {
            m_materialPtrs[meshIdx]->Bind(shader);
        }

        m_meshesPtr->at(meshIdx).Draw(renderer);

        if (hasMaterial) {
            m_materialPtrs[meshIdx]->Unbind(shader);
        }
    }
}
