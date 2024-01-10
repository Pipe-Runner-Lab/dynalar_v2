#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

#include "../../app/widgets/properties_editor.h"
#include "../utils/error.h"
#include "shader.h"
#include "texture.h"

enum class MaterialType {
    MESH_BASIC_MATERIAL,
    PHONG,
    PBR,
};

class Material : public EditorProperties {
public:
    std::string name;
    MaterialType type;

    // protected:
    //     static int MAX_TEXTURE_SLOT;

public:
    // TODO: Optimize this for single call
    Material(std::string name, MaterialType type)
        : name(name),
          type(type){
              //     GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,
              //                           &Material::MAX_TEXTURE_SLOT));
          };

    virtual void Bind(Shader& shader, int reservedTextureSlots = 0){};
    virtual void Unbind(Shader& shader){};
};

class MeshBasicMaterial : public Material {
public:
    /**
     * We are using shared pointers here since the the model owns the
     * textures, but we also have cases of null textures, so we can't use
     * references.
     */
    std::vector<std::shared_ptr<Texture>> diffuseMaps;  // aka albedoMap
    std::vector<std::shared_ptr<Texture>> specularMaps;
    std::vector<std::shared_ptr<Texture>> normalMaps;
    std::vector<std::shared_ptr<Texture>> heightMaps;

    glm::vec3 albedo = glm::vec3(1.0f);    // base color
    glm::vec3 specular = glm::vec3(1.0f);  // reflection color
    float opacity = 1.0f;
    float roughness = 0.5f;
    int metalness = 32;

    MeshBasicMaterial() : MeshBasicMaterial("Material"){};
    MeshBasicMaterial(std::string name) : Material(name, MaterialType::MESH_BASIC_MATERIAL){};

    void Bind(Shader& shader, int reservedTextureSlots = 0) override;
    void Unbind(Shader& shader) override;

    inline void AddTexture(std::shared_ptr<Texture> texturePtr) {
        if (texturePtr == nullptr) {
            throw std::runtime_error("Texture pointer is null");
        }

        switch (texturePtr->type) {
            case TextureType::DIFFUSE:
                diffuseMaps.push_back(texturePtr);
                break;
            case TextureType::SPECULAR:
                specularMaps.push_back(texturePtr);
                break;
            case TextureType::NORMAL:
                normalMaps.push_back(texturePtr);
                break;
            case TextureType::HEIGHT:
                heightMaps.push_back(texturePtr);
                break;
            default:
                throw std::runtime_error("Texture type not supported");
        }
    }

    /* ---------------------------- EDITOR PROPERTIES --------------------------- */
public:
    void RenderEditorProperties() override;
};
