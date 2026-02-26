#include <glad/glad.h>
#include "Window.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Window {

Window::Window(int width, int height, const char* title)
    : m_width(width), m_height(height), m_title(title), m_window(nullptr) {
    initialize();
}

Window::~Window() {
    shutdown();
}

void Window::initialize() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glViewport(0, 0, m_width, m_height);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    glfwSetWindowUserPointer(m_window, this);

    m_initialized = true;
}

void Window::shutdown() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::swapBuffers() {
    glfwSwapBuffers(m_window);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::setShouldClose(bool value) {
    glfwSetWindowShouldClose(m_window, value);
}

int Window::getWidth() const { return m_width; }
int Window::getHeight() const { return m_height; }
GLFWwindow* Window::getGLFWWindow() const { return m_window; }

bool Window::isKeyPressed(int key) const {
    return glfwGetKey(m_window, key) == GLFW_PRESS;
}

Input::InputType Window::getKey(int keyCode) {
    return glfwGetKey(m_window, keyCode) == GLFW_PRESS ? Input::InputType::PRESS : Input::InputType::RELEASE;
}
Input::InputType Window::getMouseButton(int button) const {
    return glfwGetMouseButton(m_window, button) == GLFW_PRESS
        ? Input::InputType::PRESS
        : Input::InputType::RELEASE;
}

void Window::getCursorPos(double& x, double& y) const {
    glfwGetCursorPos(m_window, &x, &y);
}
void Window::framebufferSizeCallback(GLFWwindow* glfwWindow, int width, int height) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
    if (window) {
        window->m_width = width;
        window->m_height = height;
        glViewport(0, 0, width, height);
    }
}

void Window::setInputMode(int mode, int value) {
    glfwSetInputMode(m_window, mode, value);
}
}