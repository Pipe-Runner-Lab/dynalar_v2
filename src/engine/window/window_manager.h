#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>

#include <array>

#include "../core/utils/error.h"

struct Dimensions {
    int width;
    int height;
};

struct MousePosition {
    double x;
    double y;
    bool isInitialized = false;
};

struct MousePositionDelta {
    double x;
    double y;
};

class WindowManager {
private:
    static const unsigned int KEYCODE_UPPER_LIMIT = 1024;
    static const unsigned int KEYCODE_LOWER_LIMIT = 0;
    std::array<bool, KEYCODE_UPPER_LIMIT> m_keys;
    // size based on https://www.glfw.org/docs/3.3/group__buttons.html
    std::array<bool, 8> m_mouseButtons;

    GLFWwindow *m_windowPtr;
    int m_currentHeight = 0, m_currentWidth = 0;
    float m_currentAspectRatio = 1.0f;
    MousePosition m_mousePosition = {0, 0};
    MousePositionDelta m_mousePositionDelta = {0, 0};

public:
    WindowManager();
    ~WindowManager();

    inline bool ShouldWindowClose() {
        return glfwWindowShouldClose(m_windowPtr);
    };

    inline GLFWwindow *GetWindowPtr() {
        return m_windowPtr;
    };

    inline void CloseWindow() {
        glfwSetWindowShouldClose(m_windowPtr, true);
    };

    inline Dimensions GetDimensions() {
        return {m_currentWidth, m_currentHeight};
    };

    inline float GetAspectRatio() {
        return m_currentAspectRatio;
    };

    // keyboard
    inline void SetKey(unsigned int keycode, bool value) {
        m_keys[keycode] = value;
    };
    inline bool GetKey(unsigned int keycode) {
        return m_keys[keycode];
    };

    // mouse
    inline void SetMouseButton(unsigned int keycode, bool value) {
        m_mouseButtons[keycode] = value;
    };
    inline bool GetMouseButton(unsigned int keycode) {
        return m_mouseButtons[keycode];
    };
    inline void ResetMouse() {
        m_mousePosition.x = 0;
        m_mousePosition.y = 0;
        m_mousePosition.isInitialized = false;
    };
    inline void HideCursor() {
        glfwSetInputMode(m_windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    };
    inline void ShowCursor() {
        glfwSetInputMode(m_windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    };
    inline MousePositionDelta GetMouseDelta() {
        MousePositionDelta mousePosition = m_mousePositionDelta;
        m_mousePositionDelta.x = 0;
        m_mousePositionDelta.y = 0;
        return mousePosition;
    };

    void SetViewport(int width, int height);
    void ResetViewport();

private:
    static void FrameBufferResizeCallback(GLFWwindow *window_ptr, int width, int height);
    static void KeyCallback(GLFWwindow *window_ptr, int key, int code, int action, int mode);
    static void MouseCursorCallback(GLFWwindow *window_ptr, double xPos, double yPos);
    static void MouseButtonCallback(GLFWwindow *window_ptr, int button, int action, int mods);
};