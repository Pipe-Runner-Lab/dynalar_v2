#include "fps_graph.h"

FPSGraph::FPSGraph(WindowManager &windowManager, bool &open) : m_windowManager(windowManager), m_open(open)
{
}

void FPSGraph::Render()
{
  if (!m_open)
    return;

  int fps = ImGui::GetIO().Framerate;

  // Get m_frames
  if (m_frames.size() > 100) // Max seconds to show
  {
    for (size_t i = 1; i < m_frames.size(); i++)
    {
      m_frames[i - 1] = m_frames[i];
    }
    m_frames[m_frames.size() - 1] = fps;
  }
  else
  {
    m_frames.push_back(fps);
  }
  ImGui::SetNextWindowPos(ImVec2(40, 40 + 40));
  ImGui::Begin("FPS Graph", &m_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

  std::string text = fmt::format("Frames: {}", fps);
  ImGui::Text("%s", text.c_str());

  ImGui::PlotHistogram("Framerate", &m_frames[0], m_frames.size(), 0, NULL, 0.0f, 100.0f, ImVec2(300, 100));
  if (ImGui::SliderInt("Max FPS", &m_windowManager.max_fps, 60, 120, NULL))
  {
    // TODO: Implement
  }
  ImGui::End();
}
