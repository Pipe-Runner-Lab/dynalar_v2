#include <vector>

#include "../../engine/app/base_scene.h"

class HelloTextureScene : public BaseScene {
private:
    Shader m_shader;
    Texture m_texture;

public:
    HelloTextureScene(RenderContext &renderContext);
    void OnRender() override;
};