#pragma once

#include <GL/glew.h>
#include <fmt/core.h>
#include <GLFW/glfw3.h>

struct Dimensions
{
  int width;
  int height;
};

class WindowManager
{
private:
  GLFWwindow *m_windowPtr;
  int currentHeight = 0, currentWidth = 0;
  float currentAspectRatio = 1.0f;

public:
  int max_fps = 60;

public:
  WindowManager();
  ~WindowManager();

  inline bool ShouldWindowClose()
  {
    return glfwWindowShouldClose(m_windowPtr);
  };

  inline GLFWwindow *GetWindowPtr()
  {
    return m_windowPtr;
  };

  inline void CloseWindow()
  {
    glfwSetWindowShouldClose(m_windowPtr, true);
  };

  inline Dimensions GetDimensions()
  {
    return {currentWidth, currentHeight};
  };

  inline float GetAspectRatio()
  {
    return currentAspectRatio;
  };

private:
  static void FrameBufferResizeCallback(GLFWwindow *window_ptr, int width, int height);
};