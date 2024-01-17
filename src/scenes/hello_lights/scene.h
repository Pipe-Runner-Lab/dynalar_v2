#pragma once

#include <vector>

#include "../../engine/app/base_scene.h"
#include "../../engine/app/lights/ambient_light.h"
#include "../../engine/app/pre_made_objects/cube.h"
#include "../../engine/app/pre_made_objects/plane.h"
#include "../../engine/app/widgets/properties_editor.h"
#include "../../engine/core/gl/shadow_map.h"

class HelloLightsScene : public BaseScene {
public:
    HelloLightsScene(RenderContext &renderContext);
    void OnUpdate() override;
    void OnRender() override;
};