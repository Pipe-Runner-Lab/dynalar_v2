#include "model.h"

Model::Model(std::string title, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : title(title),
      m_position(position),
      m_rotation(rotation),
      m_scale(scale),
      m_meshesPtr(std::make_shared<std::vector<Mesh>>()) {
}

Model::Model(std::string title, std::vector<Mesh> &meshGroup, glm::vec3 position,
             glm::vec3 rotation, glm::vec3 scale)
    : title(title),
      m_meshesPtr(std::make_shared<std::vector<Mesh>>(std::move(meshGroup))),
      m_position(position),
      m_rotation(rotation),
      m_scale(scale) {
}

Model::Model(std::string title, std::vector<Mesh> &meshGroup,
             std::vector<std::shared_ptr<Material>> &materialPtrs, glm::vec3 position,
             glm::vec3 rotation, glm::vec3 scale)
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

Model::Model(std::string title, std::string path, glm::vec3 position, glm::vec3 rotation,
             glm::vec3 scale)
    : m_path(path),
      m_directory(path.substr(0, path.find_last_of('/'))),
      title(title),
      m_position(position),
      m_rotation(rotation),
      m_scale(scale),
      m_meshesPtr(std::make_shared<std::vector<Mesh>>()) {
    Assimp::Importer importer;

    // TODO: This is a hacky solution for now
    bool isGLTF = path.substr(path.find_last_of('.') + 1) == "gltf";

    const aiScene *scene =
        importer.ReadFile(path, aiProcess_Triangulate | (isGLTF ? 0 : aiProcess_FlipUVs));

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error(fmt::format("Assimp error - {}.\n", importer.GetErrorString()));
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
    std::shared_ptr<MeshBasicMaterial> materialPtr = std::make_shared<MeshBasicMaterial>();

    aiTextureType textureTypes[2] = {aiTextureType_DIFFUSE, aiTextureType_SPECULAR};

    // iterate over all texture types
    for (auto &textureType : textureTypes) {
        for (unsigned int i = 0; i < mat->GetTextureCount(textureType); i++) {
            std::shared_ptr<Texture> texturePtr = nullptr;
            aiString str;
            mat->GetTexture(textureType, i, &str);
            std::string texturePath = m_directory + "/" + str.C_Str();

            // if texture in cache, use it, else create it and add to cache
            if (texturePtrCache.find(texturePath) != texturePtrCache.end()) {
                texturePtr = texturePtrCache[texturePath];
            } else {
                texturePtr = std::make_shared<Texture>(texturePath, TextureType(textureType));
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

void Model::Draw(Renderer &renderer, Shader &shader, glm::mat4 &vpMatrix, int reservedTextureSlots,
                 RenderPassType renderPassType) {
    glm::mat4 &modelMatrix = GetModelMatrix();

    if (renderPassType == RenderPassType::SHADOW) {
        shader.SetUniformMatrix4f("u_mvpMatrix", Renderer::ComputeMVPMatrix(vpMatrix, modelMatrix));
        ShadowPass(renderer, shader);
        return;
    }

    shader.SetUniformMatrix4f("u_mvpMatrix", Renderer::ComputeMVPMatrix(vpMatrix, modelMatrix));
    shader.SetUniformMatrix4f("u_mMatrix", modelMatrix);
    shader.SetUniformBool("u_debugNormals", debugNormals);

    Draw(renderer, shader, reservedTextureSlots);
}

void Model::Draw(Renderer &renderer, Shader &shader, int reservedTextureSlots) {
    bool hasMaterial = m_materialPtrs.size() > 0;

    if (hasMaterial && m_materialPtrs.size() != m_meshesPtr->size()) {
        throw std::runtime_error(
            fmt::format("Material count ({}) does not match mesh count ({}).\n",
                        m_materialPtrs.size(), m_meshesPtr->size()));
    }

    for (int meshIdx = 0; meshIdx < m_meshesPtr->size(); meshIdx++) {
        if (wireframe) {
            GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        } else {
            GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        }

        if (hasMaterial) {
            m_materialPtrs[meshIdx]->Bind(shader, reservedTextureSlots);
        }

        m_meshesPtr->at(meshIdx).Draw(renderer);

        if (hasMaterial) {
            m_materialPtrs[meshIdx]->Unbind(shader);
        }
    }
}

void Model::ShadowPass(Renderer &renderer, Shader &shader) {
    for (int meshIdx = 0; meshIdx < m_meshesPtr->size(); meshIdx++) {
        m_meshesPtr->at(meshIdx).Draw(renderer);
    }
}

void Model::RenderEditorProperties() {
    ImGui::Checkbox("Debug Normals", &debugNormals);
    ImGui::Checkbox("Wireframe", &wireframe);

    ImGui::SeparatorText("Object Properties");
    ImGui::DragFloat3("Position", glm::value_ptr(m_position), 0.1f);
    ImGui::DragFloat3("Rotation", glm::value_ptr(m_rotation), 0.1f, -360.0f, 360.0f);
    ImGui::DragFloat3("Scale", glm::value_ptr(m_scale), 0.1f, 0.001f);

    ImGui::SeparatorText("Mesh Properties");
    std::vector<Mesh> &meshes = *m_meshesPtr;

    if (meshes.size() <= 0) {
        ImGui::Text("No meshes");
        return;
    }

    if (ImGui::BeginCombo("Selected Mesh", fmt::format("Mesh {}", m_selectedMeshIdx).c_str())) {
        for (int itemIdx = 0; itemIdx < meshes.size(); itemIdx++) {
            const bool is_selected = (m_selectedMeshIdx == itemIdx);
            if (ImGui::Selectable(fmt::format("Mesh {}", itemIdx).c_str()), is_selected)
                m_selectedMeshIdx = itemIdx;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (m_materialPtrs.size() == 0) {
        ImGui::Text("No material attached to the model!");
        return;
    }

    Mesh &activeMesh = meshes[m_selectedMeshIdx];
    std::shared_ptr<Material> materialPtr = m_materialPtrs[m_selectedMeshIdx];
    if (materialPtr) {
        materialPtr->RenderEditorProperties();
    } else {
        ImGui::Text("No material attached to the mesh!");
    }
}
