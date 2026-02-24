#pragma once

#include <GLFW/glfw3.h>
#include "InputType.h"

namespace Window {

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();

    void initialize();
    void shutdown();

    void pollEvents();
    void swapBuffers();
    bool shouldClose() const;
    void setShouldClose(bool value);

    int getWidth() const;
    int getHeight() const;
    GLFWwindow* getGLFWWindow() const;

    bool isKeyPressed(int key) const;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    bool isInitialized() const { return m_initialized; }

    Input::InputType getKey(int keyCode);
    Input::InputType getMouseButton(int button) const;
    void getCursorPos(double& x, double& y) const;

private:
    int m_width;
    int m_height;
    const char* m_title;
    GLFWwindow* m_window;
    bool m_initialized = false;
};

} // namespace vengine
