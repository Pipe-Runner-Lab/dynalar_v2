#include <memory>
#include "../core/gl/renderer.h"
#include "../window/window_manager.h"
#include "scene_manager.h"
#include "../window/gui.h"
#include "./base_scene.h"

class App
{
private:
  RenderContext m_renderContext;
  GUI m_gui;

public:
  App();

  void StartRenderLoop(SceneManager &m_sceneManager);
  inline RenderContext &GetRenderContext() { return m_renderContext; };
};