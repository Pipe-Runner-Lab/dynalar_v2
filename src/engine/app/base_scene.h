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
#include "lights/point_light.h"

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
    unsigned int ambientLightCount = 0;
    unsigned int pointLightCount = 0;
    std::vector<std::unique_ptr<BaseLight>> m_lightPtrs;

    void Bind(Shader &shader) {
        shader.SetUniform1i("u_numPointLights", pointLightCount);

        int pointLightIdx = 0;
        for (auto &lightPtr : m_lightPtrs) {
            switch (lightPtr->type) {
                case LightType::AMBIENT:
                    lightPtr->Bind(shader);
                    break;
                case LightType::POINT:
                    lightPtr->Bind(shader, pointLightIdx);
                    pointLightIdx++;
                    break;
                default:
                    break;
            }
        }
    }

    void Unbind(Shader &shader) {
        shader.SetUniform1i("u_numPointLights", 0);

        int pointLightIdx = 0;
        for (auto &lightPtr : m_lightPtrs) {
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
    std::vector<Model> m_models;
    LightsContainer m_lightsContainer;

private:
    std::vector<Camera> m_cameras;
    int m_activeCameraIndex = 0;

    int m_activeModelIndex = 0;
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
    void AddCamera(Camera &camera) {
        m_cameras.push_back(std::move(camera));
    }

    void AddCamera(Camera &&camera) {
        m_cameras.push_back(std::move(camera));
    }

    Camera &GetActiveCamera() {
        if (m_cameras.size() == 0) {
            throw std::runtime_error("No camera has been added to the scene");
        }

        return m_cameras[m_activeCameraIndex];
    }

    void SetActiveCameraIndex(int index) {
        if (index < m_cameras.size()) {
            m_activeCameraIndex = index;
        }
    }

    void AddModel(Model &model) {
        m_models.push_back(std::move(model));
    }

    void AddModel(Model &&model) {
        m_models.push_back(std::move(model));
    }

    void AddLight(std::unique_ptr<BaseLight> &&lightPtr) {
        switch (lightPtr->type) {
            case LightType::AMBIENT: {
                m_lightsContainer.ambientLightCount++;

                if (m_lightsContainer.ambientLightCount > 1) {
                    throw std::runtime_error(
                        "Only one ambient light can be added to the scene");
                }
                break;
            };
            case LightType::POINT: {
                m_lightsContainer.pointLightCount++;
                break;
            };
            default:
                break;
        }

        m_lightsContainer.m_lightPtrs.push_back(std::move(lightPtr));
    }

    Model &GetActiveModel() {
        if (m_models.size() == 0) {
            throw std::runtime_error("No model has been added to the scene");
        }

        return m_models[m_activeModelIndex];
    }

    std::unique_ptr<BaseLight> &GetActiveLightPtr() {
        if (m_lightsContainer.m_lightPtrs.size() == 0) {
            throw std::runtime_error("No light has been added to the scene");
        }

        return m_lightsContainer.m_lightPtrs[m_activeLightIndex];
    }
};
