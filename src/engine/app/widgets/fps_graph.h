#pragma once

#include <fmt/core.h>
#include <imgui.h>

#include <vector>

#include "../base_scene.h"

// * Credit: https://github.com/CITMProject3/Project3/blob/master/FPSGraph.cpp
class FPSGraph {
private:
    RenderContext &m_renderCtx;

public:
    FPSGraph(RenderContext &renderCtx, bool &open);
    void Render();

private:
    bool &m_open;
    std::vector<float> m_frames;
};