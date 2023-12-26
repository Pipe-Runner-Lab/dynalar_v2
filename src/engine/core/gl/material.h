#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "shader.h"
#include "texture.h"

enum class MaterialType {
    MESH_BASIC_MATERIAL,
    PHONG,
    PBR,
};

class Material {
public:
    std::string name;
    MaterialType type;

public:
    Material(std::string name, MaterialType type) : name(name), type(type){};
    virtual void Bind(Shader& shader){};
    virtual void Unbind(Shader& shader){};
};

class MeshBasicMaterial : public Material {
public:
    std::shared_ptr<Texture> diffuseMap = nullptr;  // aka albedoMap
    std::shared_ptr<Texture> normalMap = nullptr;
    std::shared_ptr<Texture> specularMap = nullptr;
    std::shared_ptr<Texture> alphaMap = nullptr;
    std::shared_ptr<Texture> displacementMap = nullptr;
    std::shared_ptr<Texture> roughnessMap = nullptr;
    std::shared_ptr<Texture> metalnessMap = nullptr;
    std::shared_ptr<Texture> aoMap = nullptr;

    glm::vec4 albedo = glm::vec4(1.0f);  // base color
    float opacity = 1.0f;
    float roughness = 0.5f;
    float metalness = 0.5f;
    float ao = 1.0f;

    MeshBasicMaterial() : MeshBasicMaterial("Material"){};
    MeshBasicMaterial(std::string name)
        : Material(name, MaterialType::MESH_BASIC_MATERIAL){};

    void Bind(Shader& shader) override;
    void Unbind(Shader& shader) override;
};
