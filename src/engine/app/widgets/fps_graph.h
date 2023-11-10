#pragma once

#include <vector>
#include <fmt/core.h>
#include <imgui.h>
#include "../../window/window_manager.h"

// * Credit: https://github.com/CITMProject3/Project3/blob/master/FPSGraph.cpp
class FPSGraph
{
private:
  WindowManager &m_windowManager;

public:
  FPSGraph(WindowManager &windowManager, bool &open);
  void Render();

private:
  bool &m_open;
  std::vector<float> m_frames;
};