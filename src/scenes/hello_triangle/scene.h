#include <vector>
#include "../../engine/app/base_scene.h"

class HelloTriangleScene : public BaseScene
{
private:
  Shader m_shader;
  std::unique_ptr<Model> m_modelPtr;

public:
  HelloTriangleScene(RenderContext &renderContext);
  void OnRender() override;
};