#include <fmt/core.h>

#include "engine/app/app.h"
#include "scenes/hello_3D_world/scene.h"
#include "scenes/hello_texture/scene.h"
#include "scenes/hello_triangle/scene.h"

int main(int, char **) {
    try {
        App app = App();
        app.sceneManager.RegisterScene<HelloTriangleScene>("Hello Triangle");
        app.sceneManager.RegisterScene<HelloTextureScene>("Hello Texture");
        app.sceneManager.RegisterScene<Hello3DWorldScene>("Hello 3D World");
        app.StartRenderLoop();
    } catch (const std::runtime_error &e) {
        fmt::print(e.what());
        exit(-1);
    }

    return 0;
}
