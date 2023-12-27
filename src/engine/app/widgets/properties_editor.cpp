#include "properties_editor.h"

ObjectPropertiesEditor::ObjectPropertiesEditor(std::function<void()> renderCb)
    : BasePropertiesEditor(renderCb) {
}

void ObjectPropertiesEditor::SetAdditionalRenderCallback(
    std::function<void()> renderCb) {
    m_additionalRenderCb = renderCb;
}

void ObjectPropertiesEditor::Render() {
    if (ImGui::BeginTabItem("Object Properties")) {
        m_renderCb();
        m_additionalRenderCb();
        ImGui::EndTabItem();
    }
}

CameraPropertiesEditor::CameraPropertiesEditor(std::function<void()> renderCb)
    : BasePropertiesEditor(renderCb) {
}

void CameraPropertiesEditor::Render() {
    if (ImGui::BeginTabItem("Camera")) {
        m_renderCb();
        ImGui::EndTabItem();
    }
}

InputPropertiesEditor::InputPropertiesEditor(std::function<void()> renderCb)
    : BasePropertiesEditor(renderCb) {
}

void InputPropertiesEditor::Render() {
    if (ImGui::BeginTabItem("Input")) {
        m_renderCb();
        ImGui::EndTabItem();
    }
}

LightPropertiesEditor::LightPropertiesEditor(std::function<void()> renderCb)
    : BasePropertiesEditor(renderCb) {
}

void LightPropertiesEditor::Render() {
    if (ImGui::BeginTabItem("Lights")) {
        m_renderCb();
        ImGui::EndTabItem();
    }
}
