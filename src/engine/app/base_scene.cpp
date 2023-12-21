#include "base_scene.h"

BaseScene::BaseScene(RenderContext &renderContext, std::string sceneTitle)
    : m_renderContext(renderContext), m_sceneTitle(sceneTitle) {
    m_objectPropertiesEditorPtr =
        std::make_shared<ObjectPropertiesEditor>(ObjectPropertiesEditor([&]() {
            ImGui::Text("%s", m_sceneTitle.c_str());

            if (ImGui::BeginCombo(
                    "Selected Object",
                    fmt::format("Object {}", m_activeModelIndex).c_str())) {
                for (int itemIdx = 0; itemIdx < m_models.size(); itemIdx++) {
                    const bool is_selected = (m_activeModelIndex == itemIdx);
                    if (ImGui::Selectable(
                            fmt::format("Object {}", itemIdx).c_str(),
                            is_selected))
                        m_activeModelIndex = itemIdx;

                    // TODO: When object is selected, disable animation
                    if (is_selected) {
                        // disable animation
                    } else {
                        // enable animation
                    }
                }
                ImGui::EndCombo();
            }

            if (m_activeModelIndex < 0)
                return;
            Model &activeModel = GetActiveModel();
            ImGui::Checkbox("Debug Normals", &activeModel.debugNormals);
        }));

    m_cameraPropertiesEditorPtr =
        std::make_shared<CameraPropertiesEditor>(CameraPropertiesEditor([&]() {
            if (ImGui::BeginCombo(
                    "Active Camera",
                    fmt::format("Camera {}", m_activeCameraIndex).c_str())) {
                for (int itemIdx = 0; itemIdx < m_cameras.size(); itemIdx++) {
                    const bool is_selected = (m_activeCameraIndex == itemIdx);
                    if (ImGui::Selectable(
                            fmt::format("Camera {}", itemIdx).c_str(),
                            is_selected))
                        m_activeCameraIndex = itemIdx;

                    // Set the initial focus when opening the combo
                    // (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::SeparatorText("Camera Properties");
        }));

    m_inputPropertiesEditorPtr =
        std::make_shared<InputPropertiesEditor>(InputPropertiesEditor([&]() {
            ImGui::SeparatorText("Mouse");
            ImGui::SliderFloat("X Sensitivity", &m_xSensitivity, 0.01f, 30.0f);
            ImGui::SliderFloat("Y Sensitivity", &m_ySensitivity, 0.01f, 30.0f);
            ImGui::SliderFloat("Movement Sensitivity", &m_moveSpeed, 0.01f,
                               30.0f);
            ImGui::Checkbox("Inverted", &isInverted);
        }));
}

void BaseScene::OnGUIRender() {
    if (ImGui::BeginTabBar("Editor")) {
        if (m_models.size() > 0) {
            m_objectPropertiesEditorPtr->Render();
        }

        if (m_cameras.size() > 0) {
            m_cameraPropertiesEditorPtr->Render();
        }

        m_inputPropertiesEditorPtr->Render();

        ImGui::EndTabBar();
    }
}

void BaseScene::OnUpdate() {
    if (m_cameras.size() > 0) {
        WindowManager &windowManager = *m_renderContext.windowManagerPtr;

        // mouse input
        if (windowManager.GetKey(GLFW_KEY_LEFT_ALT) &&
            windowManager.GetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
            windowManager.HideCursor();
            Camera &activeCamera = GetActiveCamera();
            MousePositionDelta mousePositionDelta =
                windowManager.GetMouseDelta();
            activeCamera.UpdateDirection(
                (isInverted ? -1 : 1) * mousePositionDelta.y * m_ySensitivity *
                    m_renderContext.deltaTime,
                mousePositionDelta.x * m_xSensitivity *
                    m_renderContext.deltaTime);
        } else if (windowManager.GetKey(GLFW_KEY_LEFT_ALT) &&
                   windowManager.GetMouseButton(GLFW_MOUSE_BUTTON_MIDDLE)) {
            windowManager.HideCursor();
            Camera &activeCamera = GetActiveCamera();
            MousePositionDelta mousePositionDelta =
                windowManager.GetMouseDelta();

            glm::vec3 positionDelta =
                activeCamera.GetRight() * (float)mousePositionDelta.x *
                    m_xSensitivity * m_renderContext.deltaTime +
                activeCamera.GetUp() * (float)mousePositionDelta.y *
                    m_ySensitivity * m_renderContext.deltaTime *
                    (isInverted ? -1.0f : 1.0f);
            activeCamera.UpdatePosition(positionDelta);
        } else if (windowManager.GetKey(GLFW_KEY_LEFT_ALT) &&
                   windowManager.GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) {
            windowManager.HideCursor();
            Camera &activeCamera = GetActiveCamera();
            MousePositionDelta mousePositionDelta =
                windowManager.GetMouseDelta();

            glm::vec3 positionDelta =
                activeCamera.GetFront() * (float)mousePositionDelta.x *
                m_xSensitivity * m_renderContext.deltaTime;
            activeCamera.UpdatePosition(positionDelta);
        } else {
            // TODO: Fix redundant calls to these functions
            windowManager.ResetMouse();
            windowManager.ShowCursor();
        }

        // keyboard input
        if (windowManager.GetKey(GLFW_KEY_W)) {
            Camera &activeCamera = GetActiveCamera();
            glm::vec3 positionDelta = activeCamera.GetFront() *
                                      m_renderContext.deltaTime * m_moveSpeed;
            activeCamera.UpdatePosition(positionDelta);
        }
        if (windowManager.GetKey(GLFW_KEY_A)) {
            Camera &activeCamera = GetActiveCamera();
            glm::vec3 positionDelta = -1.0f * activeCamera.GetRight() *
                                      m_renderContext.deltaTime * m_moveSpeed;
            activeCamera.UpdatePosition(positionDelta);
        }
        if (windowManager.GetKey(GLFW_KEY_S)) {
            Camera &activeCamera = GetActiveCamera();
            glm::vec3 positionDelta = -1.0f * activeCamera.GetFront() *
                                      m_renderContext.deltaTime * m_moveSpeed;
            activeCamera.UpdatePosition(positionDelta);
        }
        if (windowManager.GetKey(GLFW_KEY_D)) {
            Camera &activeCamera = GetActiveCamera();
            glm::vec3 positionDelta = activeCamera.GetRight() *
                                      m_renderContext.deltaTime * m_moveSpeed;
            activeCamera.UpdatePosition(positionDelta);
        }
    }
}