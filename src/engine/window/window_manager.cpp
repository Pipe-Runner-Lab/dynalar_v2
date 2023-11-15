#include "window_manager.h"

WindowManager::WindowManager() {
    std::fill(m_keys.begin(), m_keys.end(), false);
    std::fill(m_mouseButtons.begin(), m_mouseButtons.end(), false);

    /* Initialize GLFW library */
    if (!glfwInit()) {
        throw "GLFW initialization failed";
    }

    /* Setup GLFW window property */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // Set major version to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // Set minor version to 3
    glfwWindowHint(GLFW_OPENGL_PROFILE,
                   GLFW_OPENGL_CORE_PROFILE);  // Set profile to core
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                   GL_TRUE);  // Set forward compatibility to true
    // glfwWindowHint(GLFW_SCALE_TO_MONITOR, GL_TRUE);                // Set
    // scale to monitor to true (for high DPI screens)
    glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);  // Set window to maximized

    /* Create a windowed mode window and its OpenGL context */
    m_windowPtr = glfwCreateWindow(1400, 900, "Dynalar V2", NULL, NULL);

    if (!m_windowPtr) {
        glfwTerminate();
        throw "GLFW failed to create window";
    }

    glfwGetFramebufferSize(
        m_windowPtr, &m_currentWidth,
        &m_currentHeight);  // get actual sizes of the created window

    /* set aspect ratio */
    m_currentAspectRatio = (float)m_currentWidth / (float)m_currentHeight;

    /* Make the window's context current */
    glfwMakeContextCurrent(m_windowPtr);  // tells openGL which window to use

    /* Enable vsync : Sync render loop to monitor refresh rate */
    glfwSwapInterval(1);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    /* Initialize GLEW */
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        glfwTerminate();
        throw "GLEW initialization failed";
    }

    /* Setup viewport size */
    glViewport(0, 0, m_currentWidth, m_currentHeight);

    /* Setup callbacks */
    // * https://discourse.glfw.org/t/what-is-a-possible-use-of-glfwgetwindowuserpointer/1294
    glfwSetWindowUserPointer(m_windowPtr, (void *)this);
    glfwSetFramebufferSizeCallback(m_windowPtr, FrameBufferResizeCallback);
    glfwSetKeyCallback(m_windowPtr, KeyCallback);
    glfwSetCursorPosCallback(m_windowPtr, MouseCursorCallback);
    glfwSetMouseButtonCallback(m_windowPtr, MouseButtonCallback);
}

WindowManager::~WindowManager() {
    glfwDestroyWindow(m_windowPtr);
    glfwTerminate();
}

void WindowManager::FrameBufferResizeCallback(GLFWwindow *window_ptr, int width,
                                              int height) {
    if (width == 0 || height == 0)
        return;

    WindowManager *windowManagerPtr =
        (WindowManager *)glfwGetWindowUserPointer(window_ptr);

    windowManagerPtr->m_currentWidth = width;
    windowManagerPtr->m_currentHeight = height;
    windowManagerPtr->m_currentAspectRatio = (float)width / (float)height;
    glViewport(0, 0, width, height);
}

void WindowManager::KeyCallback(GLFWwindow *window_ptr, int key, int code,
                                int action, int mode) {
    WindowManager *windowManagerPtr =
        (WindowManager *)glfwGetWindowUserPointer(window_ptr);

    if (KEYCODE_LOWER_LIMIT < key && key < KEYCODE_UPPER_LIMIT) {
        if (action == GLFW_PRESS) {
            windowManagerPtr->SetKey(key, true);
        }

        if (action == GLFW_RELEASE) {
            windowManagerPtr->SetKey(key, false);
        }
    }
}

void WindowManager::MouseCursorCallback(GLFWwindow *window_ptr, double xPos,
                                        double yPos) {
    WindowManager *windowManagerPtr =
        (WindowManager *)glfwGetWindowUserPointer(window_ptr);

    MousePosition &lastMousePosition = windowManagerPtr->m_mousePosition;
    MousePositionDelta &mousePositionDelta =
        windowManagerPtr->m_mousePositionDelta;

    // check initial movement, to avoid jerky start
    if (lastMousePosition.isInitialized) {
        mousePositionDelta.x = xPos - lastMousePosition.x;
        mousePositionDelta.y = yPos - lastMousePosition.y;
    }

    lastMousePosition.x = xPos;
    lastMousePosition.y = yPos;
    lastMousePosition.isInitialized = true;
}

void WindowManager::MouseButtonCallback(GLFWwindow *window_ptr, int button,
                                        int action, int mods) {
    WindowManager *windowManagerPtr =
        (WindowManager *)glfwGetWindowUserPointer(window_ptr);

    if (action == GLFW_PRESS) {
        windowManagerPtr->SetMouseButton(button, true);
    }
    if (action == GLFW_RELEASE) {
        windowManagerPtr->SetMouseButton(button, false);
    }
}
