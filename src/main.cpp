#include <iostream>
#include <memory>
#include <fmt/core.h>
#include "engine/core/window/window_manager.h"
#include "engine/core/gl/renderer.h"

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

    while (!windowManagerPtr->ShouldWindowClose())
    {
        /* Check for user events */
        glfwPollEvents();

        /* Clear the screen */
        rendererPtr->Clear();

        /* Swap front and back buffers */
        glfwSwapBuffers(windowManagerPtr->GetWindowPtr());
    }

    return 0;
}
