#include <iostream>
#include <memory>
#include "engine/core/window/WindowManager.h"
#include "engine/core/gl/Renderer.h"

using std::unique_ptr;

int main(int, char **)
{

    WindowManager windowManager = WindowManager();
    Renderer renderer = Renderer();

    while (!windowManager.ShouldWindowClose())
    {
        /* Check for user events */
        glfwPollEvents();

        /* Clear the screen */
        renderer.Clear();

        /* Swap front and back buffers */
        glfwSwapBuffers(windowManager.GetWindowPtr());
    }

    return 0;
}
