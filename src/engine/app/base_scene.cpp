#include "base_scene.h"

BaseScene::BaseScene(RenderContext &renderContext, std::string sceneTitle)
    : m_renderContext(renderContext), m_sceneTitle(sceneTitle) {
    m_objectPropertiesEditorPtr =
        std::make_unique<ObjectPropertiesEditor>(ObjectPropertiesEditor([&]() {
            ImGui::Text("Scene Title: %s", m_sceneTitle.c_str());

            if (m_selectedModelIndex < 0)
                return;

            Model &selectedModel = GetSelectedModel();

            if (ImGui::BeginCombo("Selected Object", selectedModel.title.c_str())) {
                for (int itemIdx = 0; itemIdx < m_modelPtrs.size(); itemIdx++) {
                    const bool is_selected = (m_selectedModelIndex == itemIdx);
                    if (ImGui::Selectable(m_modelPtrs[itemIdx]->title.c_str(), is_selected)) {
                        m_selectedModelIndex = itemIdx;
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            selectedModel.RenderEditorProperties();
        }));

    m_cameraPropertiesEditorPtr =
        std::make_unique<CameraPropertiesEditor>(CameraPropertiesEditor([&]() {
            if (ImGui::BeginCombo("Active Camera",
                                  fmt::format("Camera {}", m_activeCameraIndex).c_str())) {
                for (int itemIdx = 0; itemIdx < m_cameraPtrs.size(); itemIdx++) {
                    const bool is_selected = (m_activeCameraIndex == itemIdx);
                    if (ImGui::Selectable(fmt::format("Camera {}", itemIdx).c_str(), is_selected))
                        m_activeCameraIndex = itemIdx;

                    // Set the initial focus when opening the combo
                    // (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::SeparatorText("Camera Properties");

            Camera &activeCamera = GetActiveCamera();

            ImGui::DragFloat3("Position", glm::value_ptr(activeCamera.GetPosition()), 0.1f);
            ImGui::DragFloat("Yaw", &activeCamera.m_yaw, 0.1f);
            ImGui::DragFloat("Pitch", &activeCamera.m_pitch, 0.1f);
        }));

    m_lightPropertiesEditorPtr =
        std::make_unique<LightPropertiesEditor>(LightPropertiesEditor([&]() {
            BaseLight *selectedLightPtr = GetSelectedLightPtr().get();
            auto &lightPtrs = m_lightsContainer.m_lightPtrs;

            if (ImGui::BeginCombo("Selected Light", selectedLightPtr->name.c_str())) {
                for (int itemIdx = 0; itemIdx < lightPtrs.size(); itemIdx++) {
                    const bool is_selected = (m_activeLightIndex == itemIdx);
                    if (ImGui::Selectable(lightPtrs[itemIdx]->name.c_str(), is_selected)) {
                        m_activeLightIndex = itemIdx;
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::SeparatorText("Light Properties");
            selectedLightPtr->RenderEditorProperties();
        }));

    m_inputPropertiesEditorPtr =
        std::make_unique<InputPropertiesEditor>(InputPropertiesEditor([&]() {
            ImGui::SeparatorText("Mouse");
            ImGui::SliderFloat("X Sensitivity", &m_xSensitivity, 0.01f, 30.0f);
            ImGui::SliderFloat("Y Sensitivity", &m_ySensitivity, 0.01f, 30.0f);
            ImGui::SliderFloat("Movement Sensitivity", &m_moveSpeed, 0.01f, 30.0f);
            ImGui::Checkbox("Inverted", &isInverted);
        }));
}

void BaseScene::OnGUIRender() {
    if (ImGui::BeginTabBar("Editor")) {
        if (m_modelPtrs.size() > 0) {
            m_objectPropertiesEditorPtr->Render();
        }

        if (m_cameraPtrs.size() > 0) {
            m_cameraPropertiesEditorPtr->Render();
        }

        if (m_lightsContainer.m_lightPtrs.size() > 0) {
            m_lightPropertiesEditorPtr->Render();
        }

        m_inputPropertiesEditorPtr->Render();

        ImGui::EndTabBar();
    }
}

void BaseScene::OnUpdate() {
    if (m_cameraPtrs.size() > 0) {
        WindowManager &windowManager = *m_renderContext.windowManagerPtr;

        // mouse input
        if (windowManager.GetKey(GLFW_KEY_LEFT_ALT) &&
            windowManager.GetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
            windowManager.HideCursor();
            Camera &activeCamera = GetActiveCamera();
            MousePositionDelta mousePositionDelta = windowManager.GetMouseDelta();
            activeCamera.UpdateDirection(
                (isInverted ? -1 : 1) * mousePositionDelta.y * m_ySensitivity *
                    m_renderContext.deltaTime,
                mousePositionDelta.x * m_xSensitivity * m_renderContext.deltaTime);
        } else if (windowManager.GetKey(GLFW_KEY_LEFT_ALT) &&
                   windowManager.GetMouseButton(GLFW_MOUSE_BUTTON_MIDDLE)) {
            windowManager.HideCursor();
            Camera &activeCamera = GetActiveCamera();
            MousePositionDelta mousePositionDelta = windowManager.GetMouseDelta();

            glm::vec3 positionDelta = activeCamera.GetRight() * (float)mousePositionDelta.x *
                                          m_xSensitivity * m_renderContext.deltaTime +
                                      activeCamera.GetUp() * (float)mousePositionDelta.y *
                                          m_ySensitivity * m_renderContext.deltaTime *
                                          (isInverted ? -1.0f : 1.0f);
            activeCamera.UpdatePosition(positionDelta);
        } else if (windowManager.GetKey(GLFW_KEY_LEFT_ALT) &&
                   windowManager.GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) {
            windowManager.HideCursor();
            Camera &activeCamera = GetActiveCamera();
            MousePositionDelta mousePositionDelta = windowManager.GetMouseDelta();

            glm::vec3 positionDelta = activeCamera.GetFront() * (float)mousePositionDelta.x *
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
            glm::vec3 positionDelta =
                activeCamera.GetFront() * m_renderContext.deltaTime * m_moveSpeed;
            activeCamera.UpdatePosition(positionDelta);
        }
        if (windowManager.GetKey(GLFW_KEY_A)) {
            Camera &activeCamera = GetActiveCamera();
            glm::vec3 positionDelta =
                -1.0f * activeCamera.GetRight() * m_renderContext.deltaTime * m_moveSpeed;
            activeCamera.UpdatePosition(positionDelta);
        }
        if (windowManager.GetKey(GLFW_KEY_S)) {
            Camera &activeCamera = GetActiveCamera();
            glm::vec3 positionDelta =
                -1.0f * activeCamera.GetFront() * m_renderContext.deltaTime * m_moveSpeed;
            activeCamera.UpdatePosition(positionDelta);
        }
        if (windowManager.GetKey(GLFW_KEY_D)) {
            Camera &activeCamera = GetActiveCamera();
            glm::vec3 positionDelta =
                activeCamera.GetRight() * m_renderContext.deltaTime * m_moveSpeed;
            activeCamera.UpdatePosition(positionDelta);
        }
    }
}