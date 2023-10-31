#include "renderer.h"

Renderer::Renderer()
{
  /* Enable depth test */
  GL_CALL(glEnable(GL_DEPTH_TEST));

  /* Enable blending, set blending function and equation */
  GL_CALL(glEnable(GL_BLEND));
  GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  GL_CALL(glBlendEquation(GL_FUNC_ADD));
}

void Renderer::Clear()
{
  GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Clear(float r, float g, float b, float a)
{
  GL_CALL(glClearColor(r, g, b, a));
  GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw()
{
}
