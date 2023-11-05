#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "window_manager.h"

class GUI
{
public:
  GUI(WindowManager &windowManager);
  ~GUI();

  void CreateFrame();
  void RenderFrame();
};