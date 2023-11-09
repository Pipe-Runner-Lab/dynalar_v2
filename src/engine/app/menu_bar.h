#pragma once

#include <imgui.h>
#include "../window/gui.h"
#include "../window/window_manager.h"

class MenuBar
{
private:
  GUIFeatures &m_feature;
  WindowManager &m_windowManager;

public:
  MenuBar(WindowManager &windowManager, GUIFeatures &feature);
  void Render();
};