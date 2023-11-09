#include "scene_manager.h"

SceneManager::SceneManager(RenderContext &renderContext) : m_renderContext(renderContext), m_activeScenePtr(nullptr)
{
}

void SceneManager::RenderSceneList()
{
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

  ImGui::Begin("Scene List", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
  for (auto &scene : m_scenes)
  {
    if (ImGui::Button(scene.first.c_str()))
    {
      m_activeScenePtr = scene.second();
    }
  }
  ImGui::End();
}
