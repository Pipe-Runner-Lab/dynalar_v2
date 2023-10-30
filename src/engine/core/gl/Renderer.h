#pragma once

#include <GL/glew.h>

class Renderer
{
public:
  Renderer(){};
  ~Renderer(){};

  void Clear();
  void Clear(float r, float g, float b, float a);
  void Draw(){};
};