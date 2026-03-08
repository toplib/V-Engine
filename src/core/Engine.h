#pragma once
#include <vector>

#include "rendering/Renderer.h"
#include "scene/Scene.h"
#include "window/Window.h"

namespace Engine {
    class Engine {
    public:
        Engine();
        ~Engine();

    private:
        std::vector<Scene::Scene> m_scenes;
        Scene::Scene* m_activeScene;
        Window::Window m_window;
        Rendering::Renderer m_renderer;
    };
}
