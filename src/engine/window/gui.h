#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "window_manager.h"

struct GUIFeatures
{
  bool showFPSGraph = true;
  bool stageEditor = true;
};

class GUI
{
private:
  float m_guiScale = 2.0f;

public:
  GUIFeatures features;

public:
  GUI(WindowManager &windowManager);
  ~GUI();

  void CreateFrame();
  void RenderFrame();
};