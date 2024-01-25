#include "light_manager.h"

void LightsManager::GenerateShadowMaps(Renderer& renderer, WindowManager& window_manager,
                                       std::vector<std::unique_ptr<Model>>& modelPtrs,
                                       Shader& directionalShadowShader,
                                       Shader& omniDirectionalShadowShader) {
    directionalShadowMapCount = 0;
    for (auto& lightPtr : lightPtrs) {
        switch (lightPtr->type) {
            case LightType::DIRECTIONAL: {
                DirectionalLight* dirLightPtr = static_cast<DirectionalLight*>(lightPtr.get());
                if (dirLightPtr->m_shouldRenderShadowMap) {
                    directionalShadowMapCount++;
                    directionalShadowShader.Bind();
                    dirLightPtr->GenerateShadowMap(renderer, window_manager, modelPtrs,
                                                   directionalShadowShader);
                    directionalShadowShader.Unbind();
                }
                break;
            }
            case LightType::POINT: {
                PointLight* pointLightPtr = static_cast<PointLight*>(lightPtr.get());
                if (pointLightPtr->m_shouldRenderShadowMap) {
                    omniDirectionalShadowShader.Bind();
                    pointLightPtr->GenerateShadowMap(renderer, window_manager, modelPtrs,
                                                     omniDirectionalShadowShader);
                    omniDirectionalShadowShader.Unbind();
                }
                break;
            }
            default:
                break;
        }
    }

    window_manager.ResetViewport();
}

void LightsManager::ActivateShadowMaps(Shader& shader) {
    int shadowMapSlot = 0;
    int directionalShadowMapIdx = 0;
    int omniDirectionalShadowMapIdx = 0;
    int lightIdx = 0;
    for (auto& lightPtr : lightPtrs) {
        switch (lightPtr->type) {
            case LightType::DIRECTIONAL: {
                DirectionalLight* dirLightPtr = static_cast<DirectionalLight*>(lightPtr.get());
                if (dirLightPtr->m_shouldRenderShadowMap) {
                    dirLightPtr->m_shadowMap.ActivateShadowTexture(shadowMapSlot);
                    shader.SetUniformMatrix4f(
                        fmt::format("u_lightSpaceVpMatrices[{}]", directionalShadowMapIdx),
                        dirLightPtr->GetVpMatrix());
                    shader.SetUniform1i(fmt::format("u_shadowMaps[{}]", directionalShadowMapIdx),
                                        shadowMapSlot);
                    lightVsShadowMapIndices[lightIdx] = directionalShadowMapIdx;
                    directionalShadowMapIdx++;
                    shadowMapSlot++;
                } else {
                    lightVsShadowMapIndices[lightIdx] = -1;
                }
                break;
            }
            case LightType::POINT: {
                PointLight* pointLightPtr = static_cast<PointLight*>(lightPtr.get());
                if (pointLightPtr->m_shouldRenderShadowMap) {
                    pointLightPtr->m_shadowMap.ActivateShadowTexture(shadowMapSlot);
                    shader.SetUniform1i(
                        fmt::format("u_shadowCubeMaps[{}]", omniDirectionalShadowMapIdx),
                        shadowMapSlot);
                    lightVsShadowMapIndices[lightIdx] = omniDirectionalShadowMapIdx;
                    omniDirectionalShadowMapIdx++;
                    shadowMapSlot++;
                } else {
                    lightVsShadowMapIndices[lightIdx] = -1;
                }
                break;
            }
            default: {
                lightVsShadowMapIndices[lightIdx] = -1;
                break;
            }
        }
        lightIdx++;
    }

    reservedTextureSlotCount = shadowMapSlot;
}

void LightsManager::DeactivateShadowMaps(Shader& shader) {
    for (int i = 0; i < 10; i++) {
        shader.SetUniform1i(fmt::format("u_shadowMaps[{}]", i), 0);
        shader.SetUniform1i(fmt::format("u_shadowCubeMaps[{}]", i), 0);
    }
}

void LightsManager::IncreaseLightCount(LightType type) {
    lightCounts[(int)type]++;
    lightVsShadowMapIndices.push_back(-1);  // -1 means no shadow map

    if (lightCounts[(int)LightType::AMBIENT] > 1)
        throw std::runtime_error("Only one ambient light can be added to the scene");
}

void LightsManager::Bind(Shader& shader) {
    ActivateShadowMaps(shader);

    shader.SetUniform1i("u_numDirectionalShadowMaps", directionalShadowMapCount);

    int lightIndices[4] = {0, 0, 0, 0};
    int lightIdx = 0;
    for (auto& lightPtr : lightPtrs) {
        if (!lightPtr->enabled) {
            lightIdx++;
            continue;
        }

        lightPtr->Bind(shader, lightIndices[(int)lightPtr->type]++,
                       lightVsShadowMapIndices[lightIdx++]);
    }

    shader.SetUniform1i("u_numAmbientLights", lightIndices[(int)LightType::AMBIENT]);
    shader.SetUniform1i("u_numDirectionalLights", lightIndices[(int)LightType::DIRECTIONAL]);
    shader.SetUniform1i("u_numPointLights", lightIndices[(int)LightType::POINT]);
    shader.SetUniform1i("u_numSpotLights", lightIndices[(int)LightType::SPOT]);
}

void LightsManager::Unbind(Shader& shader) {
    DeactivateShadowMaps(shader);

    shader.SetUniform1i("u_numDirectionalShadowMaps", 0);

    shader.SetUniform1i("u_numAmbientLights", 0);
    shader.SetUniform1i("u_numDirectionalLights", 0);
    shader.SetUniform1i("u_numPointLights", 0);
    shader.SetUniform1i("u_numSpotLights", 0);

    int lightIndices[4] = {0, 0, 0, 0};
    for (auto& lightPtr : lightPtrs) {
        lightPtr->Unbind(shader, lightIndices[(int)lightPtr->type]++);
    }
}
