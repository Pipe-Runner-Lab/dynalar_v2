#include "material.h"

void MeshBasicMaterial::Bind(Shader& shader, int reservedTextureSlots) {
    int textureSlot = reservedTextureSlots;  // start binding from reserved slots (0 indexed)

    if (diffuseMaps.size() > 0) {
        shader.SetUniform1i("u_material.numDiffuseMaps", diffuseMaps.size());
        for (int diffuseMapIdx = 0; diffuseMapIdx < diffuseMaps.size(); diffuseMapIdx++) {
            auto& texturePtr = diffuseMaps[diffuseMapIdx];
            texturePtr->Bind(textureSlot);
            shader.SetUniform1i(fmt::format("u_material.diffuseMaps[{}]", diffuseMapIdx),
                                textureSlot);
            textureSlot++;
        }
    } else {
        shader.SetUniform3f("u_material.albedo", albedo.r, albedo.g, albedo.b);
    }

    if (specularMaps.size() > 0) {
        shader.SetUniform1i("u_material.numSpecularMaps", specularMaps.size());
        for (int specularMapIdx = 0; specularMapIdx < specularMaps.size(); specularMapIdx++) {
            auto& texturePtr = specularMaps[specularMapIdx];
            texturePtr->Bind(textureSlot);
            shader.SetUniform1i(fmt::format("u_material.specularMaps[{}]", specularMapIdx),
                                textureSlot);
            textureSlot++;
        }
    } else {
        shader.SetUniform3f("u_material.specular", specular.r, specular.g, specular.b);
    }

    shader.SetUniform1f("u_material.opacity", opacity);
    shader.SetUniform1i("u_material.metalness", metalness);
}

void MeshBasicMaterial::Unbind(Shader& shader) {
    shader.SetUniform1i("u_material.numDiffuseMaps", 0);
    shader.SetUniform1i("u_material.numSpecularMaps", 0);
    shader.SetUniform3f("u_material.albedo", 1.0f, 1.0f, 1.0f);
    shader.SetUniform3f("u_material.specular", 1.0f, 1.0f, 1.0f);
    shader.SetUniform1i("u_material.metalness", 0);
}

void MeshBasicMaterial::RenderEditorProperties() {
    ImGui::Text("Name: %s", name.c_str());
    ImGui::Text("Type: Mesh Basic Material");
    ImGui::ColorEdit3("Albedo", glm::value_ptr(albedo));
    ImGui::ColorEdit3("Specular", glm::value_ptr(specular));
    ImGui::SliderInt("Metalness", &metalness, 0.0, 64.0);
    ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f);
}
