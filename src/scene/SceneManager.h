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
        Scene* getScene(int index);
        void removeScene(int index);
        Scene* getActiveScene();

        private:
        int m_activeSceneIndex = -1;
        std::vector<Scene> m_scenes;

        void fixActiveScene();
        Scene buildScene(); // Temp
    };
}
