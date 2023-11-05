#include <functional>
#include <memory>
#include <vector>
#include <imgui.h>
#include <fmt/core.h>
#include "base_scene.h"

class SceneMenu : public BaseScene
{
protected:
  RenderContext m_renderContext;

private:
  BaseScene *&m_activeScenePtr;
  std::vector<
      std::pair<
          std::string,
          std::function<BaseScene *()>>>
      m_scenes;

public:
  SceneMenu(BaseScene *&activeScenePtr, std::shared_ptr<WindowManager> windowManagerPtr, std::shared_ptr<Renderer> rendererPtr);
  void OnGUIRender() override;

  template <typename T>
  void RegisterScene(const std::string &sceneName)
  {
    fmt::print("Registering scene: {}\n", sceneName);
    m_scenes.push_back(std::make_pair(
        sceneName,
        [&]()
        {
          return new T(m_renderContext);
        }));
  }
};