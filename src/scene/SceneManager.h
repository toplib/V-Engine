#pragma once
#include <vector>

#include "Scene.h"

namespace Scene {
    class SceneManager {
        public:
        SceneManager();
        ~SceneManager();

        int addScene(const Scene& scene); // Returns Scene id in future it will be ResourceType::Scene
        void setActiveScene(int index);

        private:
        Scene m_activeScene;
        std::vector<Scene> m_scenes;
    };
}
