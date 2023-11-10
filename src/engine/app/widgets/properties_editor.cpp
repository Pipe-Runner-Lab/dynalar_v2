#include "properties_editor.h"

ObjectPropertiesEditor::ObjectPropertiesEditor(std::function<void()> renderCb)
    : m_renderCb(renderCb)
{
}

void ObjectPropertiesEditor::Render()
{
  if (ImGui::BeginTabItem("Object Properties"))
  {
    m_renderCb();
    ImGui::EndTabItem();
  }
}