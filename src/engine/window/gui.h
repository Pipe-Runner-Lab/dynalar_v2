#pragma once

#include <IconFontCppHeaders/IconsFontAwesome5.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "window_manager.h"

struct GUIFeatures {
    bool showFPSGraph = true;
    bool stageEditor = true;
};

class GUI {
private:
    // Change scale of GUI according to screen DPI (not automatic as of now)
    float m_guiScale = 2.0f;
    const ImWchar icons_ranges[3] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};

public:
    GUIFeatures features;

public:
    GUI(WindowManager &windowManager);
    ~GUI();

    void CreateFrame();
    void RenderFrame();
};