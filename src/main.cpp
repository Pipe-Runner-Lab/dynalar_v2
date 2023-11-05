#include <fmt/core.h>
#include "engine/app/app.h"
#include "engine/app/scene_manager.h"

#include "scenes/hello_triangle/scene.h"
#include "scenes/hello_texture/scene.h"

using std::shared_ptr;

int main(int, char **)
{
    try
    {
        App app = App();
        SceneManager sceneManager(app.GetRenderContext());
        sceneManager.RegisterScene<HelloTriangleScene>("Hello Triangle");
        sceneManager.RegisterScene<HelloTextureScene>("Hello Texture");
        app.StartRenderLoop(sceneManager);
    }
    catch (const char *e)
    {
        fmt::print(stderr, "Error: {}\n", e);
        exit(-1);
    }

    return 0;
}
