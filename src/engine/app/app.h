#include <memory>
#include "../core/gl/renderer.h"
#include "../window/window_manager.h"
#include "../window/gui.h"
#include "scene_manager.h"
#include "base_scene.h"
#include "fps_graph.h"
#include "menu_bar.h"

class App
{
private:
  RenderContext m_renderContext;
  GUI m_gui;
  FPSGraph m_fpsGraph;
  MenuBar m_menuBar;

public:
  App();

  void StartRenderLoop(SceneManager &m_sceneManager);
  inline RenderContext &GetRenderContext() { return m_renderContext; };
};