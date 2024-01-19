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
#include "lights/light_manager.h"

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

class BaseScene {
protected:
    RenderContext &m_renderContext;
    std::vector<std::unique_ptr<Model>> m_modelPtrs;
    LightsManager m_lightsManager;
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
        m_lightsManager.IncreaseLightCount(lightPtr->type);
        m_lightsManager.lightPtrs.push_back(std::move(lightPtr));
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
        if (m_lightsManager.lightPtrs.size() == 0) {
            throw std::runtime_error("No light has been added to the scene");
        }

        return m_lightsManager.lightPtrs[m_activeLightIndex];
    }
};
