#include "app.h"

App::App() : m_renderContext({std::make_shared<WindowManager>(),
                              std::make_shared<Renderer>()}),
             m_gui(*m_renderContext.windowManagerPtr),
             m_fpsGraph(*m_renderContext.windowManagerPtr, m_gui.features.showFPSGraph),
             m_menuBar(*m_renderContext.windowManagerPtr, m_gui.features)
{
}

void App::StartRenderLoop(SceneManager &m_sceneManager)
{
  BaseScene *activeScenePtr = nullptr;
  while (!m_renderContext.windowManagerPtr->ShouldWindowClose())
  {
    /* Check for user events */
    glfwPollEvents();

    /* Start the Dear ImGui frame */
    m_gui.CreateFrame();

    /* Clear the screen */
    m_renderContext.rendererPtr->Clear();

    /* Draw the menu bar */
    m_menuBar.Render();

    /* Draw the model */
    activeScenePtr = m_sceneManager.GetActiveScenePtr();
    bool shouldDeleteScene = false;
    if (activeScenePtr)
    {
      activeScenePtr->OnUpdate();
      activeScenePtr->OnRender();

      Dimensions windowDimensions = m_renderContext.windowManagerPtr->GetDimensions();
      ImGui::SetNextWindowPos(ImVec2(windowDimensions.width - 40, 40 + 40), ImGuiCond_Always, ImVec2(1.0f, 0));
      // TODO: Feature flag not working for some reason
      ImGui::Begin("Stage Editor", &(m_gui.features.stageEditor), ImGuiWindowFlags_NoMove);
      shouldDeleteScene = false;
      if (ImGui::Button("<-"))
      {
        shouldDeleteScene = true;
      }
      activeScenePtr->OnGUIRender();
      ImGui::End();

      if (shouldDeleteScene)
      {
        m_sceneManager.DeleteActiveScenePtr();
        shouldDeleteScene = false;
      }
    }
    else
    {
      m_sceneManager.RenderSceneList();
    }

    /* Draw the FPS graph */
    m_fpsGraph.Render();

    /* Draw the GUI */
    m_gui.RenderFrame();

    /* Swap front and back buffers */
    glfwSwapBuffers(m_renderContext.windowManagerPtr->GetWindowPtr());
  }

  if (activeScenePtr)
  {
    m_sceneManager.DeleteActiveScenePtr();
  }

  fmt::print("Closing application\n");
}
