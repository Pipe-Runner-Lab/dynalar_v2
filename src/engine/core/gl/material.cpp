#include "material.h"

void MeshBasicMaterial::Bind(Shader& shader) {
    if (diffuseMaps.size() > 0 | specularMaps.size() > 0) {
        shader.SetUniformBool("u_shouldUseTexture", true);

        int textureSlot = 0;

        shader.SetUniform1i("u_material.numDiffuseMaps", diffuseMaps.size());
        for (int diffuseMapIdx = 0; diffuseMapIdx < diffuseMaps.size();
             diffuseMapIdx++) {
            auto& texturePtr = diffuseMaps[diffuseMapIdx];
            texturePtr->Bind(textureSlot);
            shader.SetUniform1i(
                fmt::format("u_material.diffuseMaps[{}]", diffuseMapIdx),
                textureSlot);
            textureSlot++;
        }

        shader.SetUniform1i("u_material.numSpecularMaps", specularMaps.size());
        for (int specularMapIdx = 0; specularMapIdx < specularMaps.size();
             specularMapIdx++) {
            auto& texturePtr = specularMaps[specularMapIdx];
            texturePtr->Bind(textureSlot);
            shader.SetUniform1i(
                fmt::format("u_material.specularMaps[{}]", specularMapIdx),
                textureSlot);
            textureSlot++;
        }
    } else {
        shader.SetUniform4f("u_material.albedo", albedo.r, albedo.g, albedo.b,
                            albedo.a);
    }
}

void MeshBasicMaterial::Unbind(Shader& shader) {
    shader.SetUniform1i("u_material.numDiffuseMaps", 0);
    shader.SetUniform1i("u_material.numSpecularMaps", 0);
    shader.SetUniformBool("u_shouldUseTexture", false);
    shader.SetUniform4f("u_material.albedo", 1.0f, 1.0f, 1.0f, 1.0f);
}
