#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class WindowManager
{
private:
  GLFWwindow *m_window_ptr;

public:
  WindowManager();
  ~WindowManager();

  inline bool ShouldWindowClose()
  {
    return glfwWindowShouldClose(m_window_ptr);
  };

  inline GLFWwindow *GetWindowPtr()
  {
    return m_window_ptr;
  };
};