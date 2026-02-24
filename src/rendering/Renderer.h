#pragma once
#include "scene/Scene.h"

namespace Rendering {
    class Renderer {
    public:
        Renderer(Scene::Scene scene);
        ~Renderer();

        void render();
    private:
        Scene::Scene m_scene;
    };
}
