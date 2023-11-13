#include <fmt/core.h>

#include "engine/app/app.h"
#include "engine/app/scene_manager.h"
#include "scenes/hello_3D_world/scene.h"
#include "scenes/hello_texture/scene.h"
#include "scenes/hello_triangle/scene.h"

using std::shared_ptr;

int main(int, char **) {
    try {
        App app = App();
        // Todo: Move scene manager to app
        SceneManager sceneManager(app.GetRenderContext());
        sceneManager.RegisterScene<HelloTriangleScene>("Hello Triangle");
        sceneManager.RegisterScene<HelloTextureScene>("Hello Texture");
        sceneManager.RegisterScene<Hello3DWorldScene>("Hello 3D World");
        app.StartRenderLoop(sceneManager);
    } catch (const std::runtime_error &e) {
        fmt::print(e.what());
        exit(-1);
    }

    return 0;
}
