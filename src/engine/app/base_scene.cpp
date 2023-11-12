#include "base_scene.h"

void BaseScene::OnUpdate() {
    if (m_cameras.size() > 0) {
        WindowManager &windowManager = *m_renderContext.windowManagerPtr;

        if (windowManager.GetKey(GLFW_KEY_W) &&
            windowManager.GetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
            windowManager.ResetMouse();
            windowManager.HideCursor();
            fmt::print("{} {}", windowManager.GetMouseDelta().x,
                       windowManager.GetMouseDelta().y);
        } else {
            windowManager.ShowCursor();
        }
    }
}

void BaseScene::OnGUIRender(
    std::shared_ptr<ObjectPropertiesEditor> objectPropertiesEditorPtr) {
    if (ImGui::BeginTabBar("Editor")) {
        objectPropertiesEditorPtr->Render();

        if (m_cameras.size() > 0 && ImGui::BeginTabItem("Camera")) {
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
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Input")) {
            ImGui::SeparatorText("Mouse");
            ImGui::SliderFloat("X Sensitivity", &m_xSensitivity, 0.01f, 10.0f);
            ImGui::SliderFloat("Y Sensitivity", &m_ySensitivity, 0.01f, 10.0f);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}