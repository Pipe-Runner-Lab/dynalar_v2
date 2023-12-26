#include "app.h"

App::App()
    : m_renderContext({std::make_unique<WindowManager>(),
                       std::make_unique<Renderer>(), 0, 90}),
      m_gui(*m_renderContext.windowManagerPtr),
      m_fpsGraph(m_renderContext, m_gui.features.showFPSGraph),
      m_menuBar(*m_renderContext.windowManagerPtr, m_gui.features),
      sceneManager(m_renderContext) {
    file_system::create_directory("data");
}

void App::StartRenderLoop() {
    BaseScene *activeScenePtr = nullptr;
    // lastFPSUpdateTime is used to calculate FPS while previousTime is used to
    // limit FPS
    double lastFPSUpdateTime = glfwGetTime();
    double previousTime = lastFPSUpdateTime;
    int frameCount = 0;
    while (!m_renderContext.windowManagerPtr->ShouldWindowClose()) {
        /* Check for user events */
        glfwPollEvents();

        /* Start the Dear ImGui frame */
        m_gui.CreateFrame();

        /* Clear the screen */
        m_renderContext.rendererPtr->Clear();

        /* Draw the menu bar */
        m_menuBar.Render();

        /* Draw the model */
        activeScenePtr = sceneManager.GetActiveScenePtr();
        bool shouldDeleteScene = false;
        if (activeScenePtr) {
            activeScenePtr->OnUpdate();
            activeScenePtr->OnRender();

            Dimensions windowDimensions =
                m_renderContext.windowManagerPtr->GetDimensions();
            ImGui::SetNextWindowPos(
                ImVec2(windowDimensions.width - 40, 40 + 40), ImGuiCond_Always,
                ImVec2(1.0f, 0));

            // TODO: Feature flag not working for some reason
            ImGui::Begin("Stage Editor", &(m_gui.features.stageEditor),
                         ImGuiWindowFlags_NoMove);
            shouldDeleteScene = false;

            // TODO: Extract this into a function
            ImGui::PushStyleColor(ImGuiCol_Button,
                                  (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                                  (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                  (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));
            if (ImGui::Button(ICON_FA_CHEVRON_LEFT " Main Menu")) {
                shouldDeleteScene = true;
            }
            ImGui::PopStyleColor(3);

            activeScenePtr->OnGUIRender();
            ImGui::End();

            if (shouldDeleteScene) {
                sceneManager.DeleteActiveScenePtr();
                shouldDeleteScene = false;
            }
        } else {
            sceneManager.RenderSceneList();
        }

        /* Draw the FPS graph */
        m_fpsGraph.Render();

        /* Draw the GUI */
        m_gui.RenderFrame();

        /* Swap front and back buffers */
        glfwSwapBuffers(m_renderContext.windowManagerPtr->GetWindowPtr());

        /* Calculate FPS */
        double currentTime = glfwGetTime();
        frameCount++;
        // If a second has passed.
        if (currentTime - lastFPSUpdateTime >= 1.0) {
            m_renderContext.fps = frameCount;
            frameCount = 0;
            lastFPSUpdateTime = currentTime;
        }

        /* limit frames */
        while (currentTime < previousTime + 1.0 / m_renderContext.max_fps) {
            // sleep for delta amount of time left
            std::this_thread::sleep_for(std::chrono::milliseconds(
                (int)((previousTime + 1.0 / m_renderContext.max_fps -
                       currentTime) *
                      1000)));

            currentTime = glfwGetTime();
        }

        m_renderContext.deltaTime = currentTime - previousTime;
        previousTime = currentTime;
    }

    if (activeScenePtr) {
        sceneManager.DeleteActiveScenePtr();
    }

    fmt::print("Closing application\n");
}
