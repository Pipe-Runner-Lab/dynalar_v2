#include <iostream>
#include <memory>
#include <fmt/core.h>
#include "engine/window/window_manager.h"
#include "engine/core/gl/renderer.h"

#include "engine/core/gl/model.h"
#include "engine/core/gl/mesh.h"
#include "engine/core/gl/shader.h"

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

    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f, // 0
        0.5f, -0.5f, 0.0f,  // 1
        0.0f, 0.5f, 0.0f    // 2
    };
    std::vector<unsigned int> indices = {
        0, 1, 2};
    VertexBufferLayout layout;
    layout.Push<float>(3);
    std::vector<Mesh> meshes = {
        Mesh(vertices, indices, layout)};
    Model model = Model(meshes);

    Shader shader = Shader("assets/shaders/vertex/clip_space.vert", "assets/shaders/fragment/simple_interpolated_color.frag");

    shader.Bind();

    while (!windowManagerPtr->ShouldWindowClose())
    {
        /* Check for user events */
        glfwPollEvents();

        /* Clear the screen */
        rendererPtr->Clear();

        /* Draw the model */
        model.Draw(*rendererPtr);

        /* Swap front and back buffers */
        glfwSwapBuffers(windowManagerPtr->GetWindowPtr());
    }

    return 0;
}
