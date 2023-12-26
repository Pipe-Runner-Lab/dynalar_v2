#include "material.h"

void MeshBasicMaterial::Bind(Shader& shader) {
    shader.SetUniform4f("u_material.albedo", albedo.r, albedo.g, albedo.b,
                        albedo.a);
}

void MeshBasicMaterial::Unbind(Shader& shader) {
    shader.SetUniform4f("u_material.albedo", 1.0f, 1.0f, 1.0f, 1.0f);
}
