#include "scene_menu.h"

SceneMenu::SceneMenu(BaseScene *&activeScenePtr, std::shared_ptr<WindowManager> windowManagerPtr, std::shared_ptr<Renderer> rendererPtr) : m_activeScenePtr(activeScenePtr), m_renderContext({windowManagerPtr, rendererPtr})
{
}

void SceneMenu::OnGUIRender()
{
  for (auto &scene : m_scenes)
  {
    if (ImGui::Button(scene.first.c_str()))
    {
      m_activeScenePtr = scene.second();
    }
  }
}
