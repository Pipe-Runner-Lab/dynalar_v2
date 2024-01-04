#include "base_light.h"

void BaseLight::RenderEditorProperties() {
    ImGui::Checkbox("Enabled", &enabled);
    if (m_lightModelPtr != nullptr) {
        ImGui::Checkbox("Render Light Model", &m_render_model);
    }
    ImGui::ColorEdit3("Color", &m_color[0]);
    ImGui::SliderFloat("Ambient Intensity", &m_ambientIntensity, 0.0f, 1.0f);
}