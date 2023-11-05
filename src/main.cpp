#include <iostream>
#include <memory>
#include <fmt/core.h>
#include "engine/window/window_manager.h"
#include "engine/window/gui.h"
#include "engine/core/gl/renderer.h"
#include "engine/app/scene_menu.h"

#include "scenes/hello_triangle/scene.h"

using std::shared_ptr;

int main(int, char **)
{
    shared_ptr<WindowManager> windowManagerPtr(nullptr);
    shared_ptr<Renderer> rendererPtr(nullptr);

    try
    {
        windowManagerPtr = std::make_shared<WindowManager>();
        rendererPtr = std::make_shared<Renderer>();
    }
    catch (const char *e)
    {
        fmt::print(stderr, "Error: {}\n", e);
        exit(-1);
    }

    GUI gui = GUI(*windowManagerPtr);

    BaseScene *activeScenePtr = nullptr;
    SceneMenu sceneMenu(activeScenePtr, windowManagerPtr, rendererPtr);
    activeScenePtr = &sceneMenu;

    sceneMenu.RegisterScene<HelloTriangleScene>("Hello Triangle");

    while (!windowManagerPtr->ShouldWindowClose())
    {
        /* Check for user events */
        glfwPollEvents();

        /* Start the Dear ImGui frame */
        gui.CreateFrame();

        /* Clear the screen */
        rendererPtr->Clear();

        /* Draw the model */
        if (activeScenePtr)
        {
            activeScenePtr->OnUpdate();
            activeScenePtr->OnRender();
            activeScenePtr->OnGUIRender();
        }

        /* Draw the GUI */
        gui.RenderFrame();

        /* Swap front and back buffers */
        glfwSwapBuffers(windowManagerPtr->GetWindowPtr());
    }

    return 0;
}
