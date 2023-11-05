#include "app.h"

App::App() : m_renderContext(
                 RenderContext({std::make_shared<WindowManager>(),
                                std::make_shared<Renderer>()})),
             m_gui(GUI(*m_renderContext.windowManagerPtr))
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

    /* Draw the model */
    activeScenePtr = m_sceneManager.GetActiveScenePtr();
    bool shouldDeleteScene = false;
    if (activeScenePtr)
    {
      activeScenePtr->OnUpdate();
      activeScenePtr->OnRender();

      ImGui::Begin("Scene");
      shouldDeleteScene = false;
      if (ImGui::Button("<-"))
      {
        shouldDeleteScene = true;
      }
      activeScenePtr->OnGUIRender();
      ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
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

    /* Draw the GUI */
    m_gui.RenderFrame();

    /* Swap front and back buffers */
    glfwSwapBuffers(m_renderContext.windowManagerPtr->GetWindowPtr());
  }
}
