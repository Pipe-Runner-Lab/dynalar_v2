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
    : m_path(path),
      m_directory(path.substr(0, path.find_last_of('/'))),
      title(title),
      m_position(position),
      m_rotation(rotation),
      m_scale(scale),
      m_meshesPtr(std::make_shared<std::vector<Mesh>>()) {
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
        processMesh(mesh, scene);
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // vertex positions
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        // vertex normals
        vertices.push_back(mesh->mNormals[i].x);
        vertices.push_back(mesh->mNormals[i].y);
        vertices.push_back(mesh->mNormals[i].z);

        // vertex texture coordinates (uv)
        if (mesh->mTextureCoords[0]) {  // does the mesh contain texture
                                        // coordinates?
            // a vertex can contain up to 8 different texture coordinates. We
            // thus make the assumption that we won't use models where a vertex
            // can have multiple texture coordinates so we always take the first
            // set (0).
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        } else {  // if no texture coordinates, set them to 0
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    VertexBufferLayout layout;
    layout.Push<float>(3);  // position
    layout.Push<float>(3);  // normal
    layout.Push<float>(2);  // texture coordinates

    // if material exists, load textures, else use default material
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        m_materialPtrs.push_back(loadMaterialTextures(material));

    } else {
        m_materialPtrs.push_back(std::make_shared<MeshBasicMaterial>());
    }

    m_meshesPtr->push_back(Mesh(vertices, indices, layout));
}

std::shared_ptr<Material> Model::loadMaterialTextures(aiMaterial *mat) {
    std::shared_ptr<MeshBasicMaterial> materialPtr =
        std::make_shared<MeshBasicMaterial>();

    aiTextureType textureTypes[2] = {aiTextureType_DIFFUSE,
                                     aiTextureType_SPECULAR};

    // iterate over all texture types
    for (auto &textureType : textureTypes) {
        for (unsigned int i = 0; i < mat->GetTextureCount(textureType); i++) {
            std::shared_ptr<Texture> texturePtr = nullptr;
            aiString str;
            mat->GetTexture(textureType, i, &str);
            std::string texturePath = str.C_Str();

            // if texture in cache, use it, else create it and add to cache
            if (texturePtrCache.find(texturePath) != texturePtrCache.end()) {
                texturePtr = texturePtrCache[texturePath];
            } else {
                texturePtr = std::make_shared<Texture>(
                    texturePath, TextureType(textureType));
                texturePtrCache[texturePath] = texturePtr;
            }

            materialPtr->AddTexture(texturePtr);
        }
    }

    return materialPtr;
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

void Model::Draw(Renderer &renderer, Shader &shader, glm::mat4 &vpMatrix) {
    glm::mat4 &modelMatrix = GetModelMatrix();

    shader.SetUniformMatrix4f(
        "u_mvpMatrix", Renderer::ComputeMVPMatrix(vpMatrix, modelMatrix));
    shader.SetUniformMatrix4f("u_mMatrix", modelMatrix);
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
