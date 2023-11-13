#include <memory>

#include "../core/gl/renderer.h"
#include "../core/utils/file_system.h"
#include "../window/gui.h"
#include "../window/window_manager.h"
#include "base_scene.h"
#include "scene_manager.h"
#include "widgets/fps_graph.h"
#include "widgets/menu_bar.h"

class App {
private:
    RenderContext m_renderContext;
    GUI m_gui;
    FPSGraph m_fpsGraph;
    MenuBar m_menuBar;

public:
    SceneManager sceneManager;

public:
    App();

    void StartRenderLoop();
    inline RenderContext &GetRenderContext() {
        return m_renderContext;
    };
};