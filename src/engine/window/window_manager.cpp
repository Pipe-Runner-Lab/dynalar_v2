#include "window_manager.h"

WindowManager::WindowManager()
{
  /* Initialize GLFW library */
  if (!glfwInit())
  {
    throw "GLFW initialization failed";
  }

  /* Setup GLFW window property */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // Set major version to 3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // Set minor version to 3
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Set profile to core
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Set forward compatibility to true
  // glfwWindowHint(GLFW_SCALE_TO_MONITOR, GL_TRUE);                // Set scale to monitor to true (for high DPI screens)
  glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE); // Set window to maximized

  /* Create a windowed mode window and its OpenGL context */
  m_windowPtr = glfwCreateWindow(1400, 900, "Dynalar V2", NULL, NULL);

  if (!m_windowPtr)
  {
    glfwTerminate();
    throw "GLFW failed to create window";
  }

  glfwGetFramebufferSize(m_windowPtr, &currentWidth, &currentHeight); // get actual sizes of the created window

  /* set aspect ratio */
  currentRatio = (float)currentWidth / (float)currentHeight;

  /* Make the window's context current */
  glfwMakeContextCurrent(m_windowPtr); // tells openGL which window to use

  /* Enable vsync : Sync render loop to monitor refresh rate */
  glfwSwapInterval(1);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  /* Initialize GLEW */
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    glfwTerminate();
    throw "GLEW initialization failed";
  }

  /* Setup viewport size */
  glViewport(0, 0, currentWidth, currentHeight);

  /* Setup callbacks */
  // * https://discourse.glfw.org/t/what-is-a-possible-use-of-glfwgetwindowuserpointer/1294
  glfwSetWindowUserPointer(m_windowPtr, (void *)this);
  glfwSetFramebufferSizeCallback(m_windowPtr, FrameBufferResizeCallback);
}

WindowManager::~WindowManager()
{
  glfwDestroyWindow(m_windowPtr);
  glfwTerminate();
}

void WindowManager::FrameBufferResizeCallback(GLFWwindow *window_ptr, int width, int height)
{
  if (width == 0 || height == 0)
    return;

  fmt::print("Window resized to {}x{}\n", width, height);

  WindowManager *windowManagerPtr = (WindowManager *)glfwGetWindowUserPointer(window_ptr);

  windowManagerPtr->currentWidth = width;
  windowManagerPtr->currentHeight = height;
  windowManagerPtr->currentRatio = (float)width / (float)height;
  glViewport(0, 0, width, height);
}
