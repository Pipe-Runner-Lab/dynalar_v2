#include "menu_bar.h"

MenuBar::MenuBar(WindowManager &windowManager, GUIFeatures &feature) : m_windowManager(windowManager), m_feature(feature)
{
}

// TODO: Get actual content space to adjust viewport for OpenGL
void MenuBar::Render()
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("Quit", "Ctrl+q"))
      {
        m_windowManager.CloseWindow();
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Window"))
    {
      if (ImGui::MenuItem("FPS Graph"))
      {
        m_feature.showFPSGraph = !m_feature.showFPSGraph;
      }
      if (ImGui::MenuItem("Stage Editor"))
      {
        m_feature.stageEditor = !m_feature.stageEditor;
      }
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}
