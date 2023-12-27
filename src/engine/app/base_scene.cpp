#include "base_scene.h"

BaseScene::BaseScene(RenderContext &renderContext, std::string sceneTitle)
    : m_renderContext(renderContext), m_sceneTitle(sceneTitle) {
    m_objectPropertiesEditorPtr =
        std::make_unique<ObjectPropertiesEditor>(ObjectPropertiesEditor([&]() {
            ImGui::Text("Scene Title: %s", m_sceneTitle.c_str());

            if (m_activeModelIndex < 0)
                return;

            Model &activeModel = GetActiveModel();

            if (ImGui::BeginCombo("Selected Object",
                                  activeModel.title.c_str())) {
                for (int itemIdx = 0; itemIdx < m_models.size(); itemIdx++) {
                    const bool is_selected = (m_activeModelIndex == itemIdx);
                    if (ImGui::Selectable(m_models[itemIdx].title.c_str(),
                                          is_selected)) {
                        m_activeModelIndex = itemIdx;
                        m_activeMeshIndex = 0;
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::Checkbox("Debug Normals", &activeModel.debugNormals);

            ImGui::SeparatorText("Object Properties");
            ImGui::DragFloat3("Position",
                              glm::value_ptr(activeModel.GetPosition()), 0.1f);
            ImGui::DragFloat3("Rotation",
                              glm::value_ptr(activeModel.GetRotation()), 0.1f,
                              -360.0f, 360.0f);
            ImGui::DragFloat3("Scale", glm::value_ptr(activeModel.GetScale()),
                              0.1f, 0.001f);

            ImGui::SeparatorText("Mesh Properties");
            std::vector<Mesh> &meshes = activeModel.GetMeshes();

            if (meshes.size() <= 0) {
                ImGui::Text("No meshes");
                return;
            }

            if (ImGui::BeginCombo(
                    "Selected Mesh",
                    fmt::format("Mesh {}", m_activeMeshIndex).c_str())) {
                for (int itemIdx = 0; itemIdx < meshes.size(); itemIdx++) {
                    const bool is_selected = (m_activeMeshIndex == itemIdx);
                    if (ImGui::Selectable(
                            fmt::format("Mesh {}", itemIdx).c_str()),
                        is_selected)
                        m_activeMeshIndex = itemIdx;

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            if (activeModel.GetMaterials().size() == 0) {
                ImGui::Text("No material");
                return;
            }

            Mesh &activeMesh = meshes[m_activeMeshIndex];
            std::shared_ptr<Material> materialPtr =
                activeModel.GetMaterials()[m_activeMeshIndex];
            if (materialPtr) {
                ImGui::Text("Name: %s", materialPtr->name.c_str());

                switch (materialPtr->type) {
                    case MaterialType::MESH_BASIC_MATERIAL: {
                        std::shared_ptr<MeshBasicMaterial> material =
                            std::static_pointer_cast<MeshBasicMaterial>(
                                materialPtr);
                        ImGui::Text("Type: Mesh Basic Material");
                        ImGui::ColorEdit4("Albedo",
                                          glm::value_ptr(material->albedo));
                        break;
                    }
                    default:
                        break;
                }
            }
        }));

    m_cameraPropertiesEditorPtr =
        std::make_unique<CameraPropertiesEditor>(CameraPropertiesEditor([&]() {
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

    m_lightPropertiesEditorPtr =
        std::make_unique<LightPropertiesEditor>(LightPropertiesEditor([&]() {
            BaseLight *activeLightPtr = GetActiveLightPtr().get();
            auto &lightPtrs = m_lightsContainer.m_lightPtrs;

            if (ImGui::BeginCombo("Selected Light",
                                  activeLightPtr->name.c_str())) {
                for (int itemIdx = 0; itemIdx < lightPtrs.size(); itemIdx++) {
                    const bool is_selected = (m_activeLightIndex == itemIdx);
                    if (ImGui::Selectable(lightPtrs[itemIdx]->name.c_str(),
                                          is_selected)) {
                        m_activeLightIndex = itemIdx;
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::SeparatorText("Light Properties");

            switch (activeLightPtr->type) {
                case LightType::AMBIENT_LIGHT: {
                    auto lightPtr = static_cast<AmbientLight *>(activeLightPtr);
                    ImGui::Text("Type: Ambient Light");
                    break;
                }
                default:
                    break;
            }

            ImGui::SliderFloat("Intensity", &activeLightPtr->GetIntensity(),
                               0.0f, 1.0f);
            ImGui::ColorEdit3("Color",
                              glm::value_ptr(activeLightPtr->GetColor()));
            if (activeLightPtr->m_lightModelPtr != nullptr) {
                ImGui::Checkbox("Render Light Model",
                                &activeLightPtr->render_model);
            }
        }));

    m_inputPropertiesEditorPtr =
        std::make_unique<InputPropertiesEditor>(InputPropertiesEditor([&]() {
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

        if (m_lightsContainer.m_lightPtrs.size() > 0) {
            m_lightPropertiesEditorPtr->Render();
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