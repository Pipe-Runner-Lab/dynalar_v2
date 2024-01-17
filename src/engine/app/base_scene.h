#pragma once

#include <fmt/core.h>

#include <iostream>
#include <memory>
#include <string>

#include "../app/widgets/properties_editor.h"
#include "../core/gl/camera.h"
#include "../core/gl/material.h"
#include "../core/gl/mesh.h"
#include "../core/gl/model.h"
#include "../core/gl/shader.h"
#include "../core/gl/texture.h"
#include "../core/gl/vertex_array.h"
#include "../core/gl/vertex_buffer.h"
#include "../core/gl/vertex_buffer_layout.h"
#include "../core/utils/logger.h"
#include "../window/window_manager.h"
#include "lights/ambient_light.h"
#include "lights/base_light.h"
#include "lights/directional_light.h"
#include "lights/point_light.h"
#include "lights/spot_light.h"

// https://docs.unity3d.com/2017.3/Documentation/Manual/SceneViewNavigation.html
// TODO: Either use or remove
enum MouseInputMode {
    MOVE,
    ORBIT,
    PAN,
};

struct RenderContext {
    std::unique_ptr<WindowManager> windowManagerPtr;
    std::unique_ptr<Renderer> rendererPtr;
    int fps = 0;
    int max_fps = 60;
    float deltaTime = 0.0f;
};

struct LightsContainer {
    unsigned int lightCounts[4] = {0, 0, 0, 0};
    std::vector<int> lightVsShadowMapIndices;
    std::vector<std::unique_ptr<BaseLight>> lightPtrs;
    int shadowMapCount = 0;

    void GenerateShadowMaps(Renderer &renderer, WindowManager &window_manager,
                            std::vector<std::unique_ptr<Model>> &modelPtrs, Shader &shader) {
        shadowMapCount = 0;
        for (auto &lightPtr : lightPtrs) {
            switch (lightPtr->type) {
                case LightType::DIRECTIONAL: {
                    DirectionalLight *dirLightPtr = static_cast<DirectionalLight *>(lightPtr.get());
                    if (dirLightPtr->m_shouldRenderShadowMap) {
                        shadowMapCount++;
                        dirLightPtr->GenerateShadowMap(renderer, window_manager, modelPtrs, shader);
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }

    void ActivateShadowMaps(Shader &shader) {
        int shadowMapSlot = 0;
        int lightIdx = 0;
        for (auto &lightPtr : lightPtrs) {
            switch (lightPtr->type) {
                case LightType::DIRECTIONAL: {
                    DirectionalLight *dirLightPtr = static_cast<DirectionalLight *>(lightPtr.get());
                    shader.SetUniformMatrix4f(
                        fmt::format("u_lightSpaceVpMatrices[{}]", shadowMapSlot),
                        dirLightPtr->GetVpMatrix());
                    if (dirLightPtr->m_shouldRenderShadowMap) {
                        dirLightPtr->m_shadowMap.ActivateShadowTexture(shadowMapSlot);
                        lightVsShadowMapIndices[lightIdx] = shadowMapSlot;
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

        for (int i = 0; i < shadowMapCount; i++) {
            shader.SetUniform1i(fmt::format("u_shadowMaps[{}]", i), i);
        }
    }

    void IncreaseLightCount(LightType type) {
        lightCounts[(int)type]++;
        lightVsShadowMapIndices.push_back(-1);  // -1 means no shadow map

        if (lightCounts[(int)LightType::AMBIENT] > 1)
            throw std::runtime_error("Only one ambient light can be added to the scene");
    }

    void Bind(Shader &shader) {
        shader.SetUniform1i("u_numShadowMaps", shadowMapCount);

        shader.SetUniform1i("u_numAmbientLights", lightCounts[(int)LightType::AMBIENT]);
        shader.SetUniform1i("u_numDirectionalLights", lightCounts[(int)LightType::DIRECTIONAL]);
        shader.SetUniform1i("u_numPointLights", lightCounts[(int)LightType::POINT]);
        shader.SetUniform1i("u_numSpotLights", lightCounts[(int)LightType::SPOT]);

        int lightIndices[4] = {0, 0, 0, 0};
        int lightIdx = 0;
        for (auto &lightPtr : lightPtrs) {
            if (!lightPtr->enabled) {
                lightIdx++;
                continue;
            }

            lightPtr->Bind(shader, lightIndices[(int)lightPtr->type],
                           lightVsShadowMapIndices[lightIdx++]);
            lightIndices[(int)lightPtr->type]++;
        }
    }

    void Unbind(Shader &shader) {
        shader.SetUniform1i("u_numShadowMaps", 0);

        shader.SetUniform1i("u_numAmbientLights", 0);
        shader.SetUniform1i("u_numDirectionalLights", 0);
        shader.SetUniform1i("u_numPointLights", 0);
        shader.SetUniform1i("u_numSpotLights", 0);

        int pointLightIdx = 0;
        for (auto &lightPtr : lightPtrs) {
            switch (lightPtr->type) {
                case LightType::AMBIENT:
                    lightPtr->Unbind(shader);
                    break;
                case LightType::POINT:
                    lightPtr->Unbind(shader, pointLightIdx);
                    pointLightIdx++;
                    break;
                default:
                    break;
            }
        }
    }
};

class BaseScene {
protected:
    RenderContext &m_renderContext;
    std::vector<std::unique_ptr<Model>> m_modelPtrs;
    LightsContainer m_lightsContainer;
    std::vector<std::unique_ptr<Camera>> m_cameraPtrs;
    std::vector<std::unique_ptr<Shader>> m_shaderPtrs;

private:
    int m_activeCameraIndex = 0;

    int m_selectedModelIndex = 0;
    int m_activeMeshIndex = 0;
    int m_activeLightIndex = 0;

    float m_xSensitivity = 8.0f;
    float m_ySensitivity = 8.0f;
    float m_moveSpeed = 8.0f;
    bool isInverted = true;

    std::string m_sceneTitle;

    std::unique_ptr<ObjectPropertiesEditor> m_objectPropertiesEditorPtr;
    std::unique_ptr<CameraPropertiesEditor> m_cameraPropertiesEditorPtr;
    std::unique_ptr<InputPropertiesEditor> m_inputPropertiesEditorPtr;
    std::unique_ptr<LightPropertiesEditor> m_lightPropertiesEditorPtr;

public:
    BaseScene(RenderContext &renderContext, std::string sceneTitle);

    // * Keep in mind that even if these functions are overriden, the base
    // * functions can still be called from children
    virtual void OnUpdate();
    virtual void OnRender(){};
    virtual void OnGUIRender();

protected:
    void AddCamera(std::unique_ptr<Camera> &&cameraPtr) {
        m_cameraPtrs.push_back(std::move(cameraPtr));
    }

    Camera &GetActiveCamera() {
        if (m_cameraPtrs.size() == 0) {
            throw std::runtime_error("No camera has been added to the scene");
        }

        return *m_cameraPtrs[m_activeCameraIndex];
    }

    void SetActiveCameraIndex(int index) {
        if (index < m_cameraPtrs.size()) {
            m_activeCameraIndex = index;
        }
    }

    void AddModel(std::unique_ptr<Model> &&modelPtr) {
        m_modelPtrs.push_back(std::move(modelPtr));
    }

    void AddLight(std::unique_ptr<BaseLight> &&lightPtr) {
        m_lightsContainer.IncreaseLightCount(lightPtr->type);
        m_lightsContainer.lightPtrs.push_back(std::move(lightPtr));
    }

    void AddShader(std::unique_ptr<Shader> &&shaderPtr) {
        m_shaderPtrs.push_back(std::move(shaderPtr));
    }

    Model &GetSelectedModel() {
        if (m_modelPtrs.size() == 0) {
            throw std::runtime_error("No model has been added to the scene");
        }

        return *m_modelPtrs[m_selectedModelIndex];
    }

    std::unique_ptr<BaseLight> &GetSelectedLightPtr() {
        if (m_lightsContainer.lightPtrs.size() == 0) {
            throw std::runtime_error("No light has been added to the scene");
        }

        return m_lightsContainer.lightPtrs[m_activeLightIndex];
    }
};
