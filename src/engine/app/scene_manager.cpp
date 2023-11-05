#include "scene_manager.h"

SceneManager::SceneManager(RenderContext &renderContext) : m_renderContext(renderContext), m_activeScenePtr(nullptr)
{
}

void SceneManager::RenderSceneList()
{
  for (auto &scene : m_scenes)
  {
    if (ImGui::Button(scene.first.c_str()))
    {
      m_activeScenePtr = scene.second();
    }
  }
}
