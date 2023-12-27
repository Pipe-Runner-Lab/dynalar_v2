#pragma once

#include <vector>

#include "../../engine/app/base_scene.h"

class HelloTriangleScene : public BaseScene {
private:
    Shader m_shader;

public:
    HelloTriangleScene(RenderContext &renderContext);
    void OnRender() override;
};