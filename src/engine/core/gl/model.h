#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

#include "material.h"
#include "mesh.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"

/**
 * Note: Rotation is right-handed
 * Note: Copying a model is not allowed.
 * https://ptspts.blogspot.com/2017/02/fast-vector-append.html
 */
class Model {
protected:
    std::shared_ptr<std::vector<Mesh>> m_meshesPtr;
    glm::mat4 m_modelMatrix;

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    std::string m_directory = "";
    std::string m_path = "";

    std::vector<std::shared_ptr<Material>> m_materialPtrs;

public:
    std::string title;
    bool debugNormals = false;
    // we will use texture path as key (or anything unique)
    std::map<std::string, std::shared_ptr<Texture>> texturePtrCache;
    // the key is the index of the mesh in the model

public:
    Model(std::string title, glm::vec3 position = glm::vec3(0.0f),
          glm::vec3 rotation = glm::vec3(0.0f),
          glm::vec3 scale = glm::vec3(1.0f));

    Model(std::string title, std::vector<Mesh> &meshGroup,
          glm::vec3 position = glm::vec3(0.0f),
          glm::vec3 rotation = glm::vec3(0.0f),
          glm::vec3 scale = glm::vec3(1.0f));

    Model(std::string title, std::vector<Mesh> &meshGroup,
          std::vector<std::shared_ptr<Material>> &materialPtrs,
          glm::vec3 position = glm::vec3(0.0f),
          glm::vec3 rotation = glm::vec3(0.0f),
          glm::vec3 scale = glm::vec3(1.0f));

    /// @brief This constructor is used for loading models from file
    Model(std::string title, std::string path,
          glm::vec3 position = glm::vec3(0.0f),
          glm::vec3 rotation = glm::vec3(0.0f),
          glm::vec3 scale = glm::vec3(1.0f));

    Model(const Model &other) = delete;

    Model(Model &&other);

    ~Model();

    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
                                              aiTextureType type,
                                              std::string typeName);

    /// @brief Applies delta translation to the model
    /// @param translation
    /// @return the model itself
    inline Model &Translate(glm::vec3 translation) {
        m_position += translation;
        return *this;
    }

    /// @brief Sets the position of the model
    inline Model &SetPosition(glm::vec3 position) {
        m_position = position;
        return *this;
    }

    /// @brief Rotates the model by delta along x, y, z axis. Rotation is
    /// applied in xyz order
    /// @param rotation A vec3 containing angles in degree for each axis
    inline Model &Rotate(glm::vec3 rotation) {
        m_rotation += rotation;
        return *this;
    }

    /// @brief Sets the rotation of the model
    inline Model &SetOrientation(glm::vec3 rotation) {
        m_rotation = rotation;
        return *this;
    }

    inline Model &Scale(glm::vec3 scale) {
        m_scale += scale;
        return *this;
    }

    inline Model &SetScale(glm::vec3 scale) {
        m_scale = scale;
        return *this;
    }

    void Draw(Renderer &renderer, Shader &shader, glm::mat4 vpMatrix);

    void Draw(Renderer &renderer, Shader &shader);

    inline glm::mat4 &GetModelMatrix() {
        m_modelMatrix = glm::mat4(1.0f);
        m_modelMatrix = glm::translate(m_modelMatrix, m_position);
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.x),
                                    glm::vec3(1.0f, 0.0f, 0.0f));
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.y),
                                    glm::vec3(0.0f, 1.0f, 0.0f));
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.z),
                                    glm::vec3(0.0f, 0.0f, 1.0f));
        m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
        return m_modelMatrix;
    }

    inline glm::vec3 &GetPosition() {
        return m_position;
    }

    inline glm::vec3 &GetRotation() {
        return m_rotation;
    }

    inline glm::vec3 &GetScale() {
        return m_scale;
    }

    inline std::vector<Mesh> &GetMeshes() {
        return *m_meshesPtr;
    }

    inline std::vector<std::shared_ptr<Material>> &GetMaterials() {
        return m_materialPtrs;
    }

    // TODO: Implement this
    static void PhongShadingConverter(std::vector<Mesh> &meshGroup){};
};