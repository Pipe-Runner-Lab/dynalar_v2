#pragma once

#include <fmt/core.h>

#include <iostream>
#include <memory>
#include <string>

#include "../app/widgets/properties_editor.h"
#include "../core/gl/camera.h"
#include "../core/gl/mesh.h"
#include "../core/gl/model.h"
#include "../core/gl/shader.h"
#include "../core/gl/texture.h"
#include "../core/gl/vertex_array.h"
#include "../core/gl/vertex_buffer.h"
#include "../core/gl/vertex_buffer_layout.h"
#include "../window/window_manager.h"

// https://docs.unity3d.com/2017.3/Documentation/Manual/SceneViewNavigation.html
enum MouseInputMode {
    MOVE,
    ORBIT,
    PAN,
};

struct RenderContext {
    std::shared_ptr<WindowManager> windowManagerPtr;
    std::shared_ptr<Renderer> rendererPtr;
};

class BaseScene {
protected:
    RenderContext &m_renderContext;

private:
    std::vector<Camera> m_cameras;
    int m_activeCameraIndex = 0;
    float m_xSensitivity = 0.1f;
    float m_ySensitivity = 0.1f;

public:
    BaseScene(RenderContext &renderContext) : m_renderContext(renderContext){};

    // * Keep in mind that even if these functions are overriden, the base
    // * functions can still be called from children
    virtual void OnUpdate();
    virtual void OnRender(){};
    virtual void OnGUIRender(){};

protected:
    // this has intentionally been made non overrideable with a change in
    // signature
    void OnGUIRender(
        std::shared_ptr<ObjectPropertiesEditor> objectPropertiesEditorPtr);

    void AddCamera(Camera camera) {
        m_cameras.push_back(std::move(camera));
    }

    Camera &GetActiveCamera() {
        return m_cameras[m_activeCameraIndex];
    }

    void SetActiveCameraIndex(int index) {
        if (index < m_cameras.size()) {
            m_activeCameraIndex = index;
        }
    }
};
