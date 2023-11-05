#pragma once

#include <iostream>
#include <memory>
#include <string>
#include "../core/gl/model.h"
#include "../core/gl/mesh.h"
#include "../core/gl/shader.h"
#include "../core/gl/vertex_array.h"
#include "../core/gl/vertex_buffer.h"
#include "../core/gl/vertex_buffer_layout.h"
#include "../window/window_manager.h"

struct RenderContext
{
  std::shared_ptr<WindowManager> windowManagerPtr;
  std::shared_ptr<Renderer> rendererPtr;
};

class BaseScene
{
protected:
  RenderContext *m_renderContextPtr = nullptr;

public:
  BaseScene(){};
  BaseScene(RenderContext *renderContextPtr) : m_renderContextPtr(renderContextPtr){};
  virtual ~BaseScene(){};

  virtual void OnUpdate(){};
  virtual void OnRender(){};
  virtual void OnGUIRender(){};
};
