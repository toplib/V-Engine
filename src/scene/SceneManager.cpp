#include "SceneManager.h"

#include <algorithm>
#include <stdexcept>

namespace Scene {
    SceneManager::SceneManager() {
        fixActiveScene(); // Set default active scene
    };
    SceneManager::~SceneManager() = default;

    int SceneManager::addScene(const Scene& scene) {
        m_scenes.push_back(scene);
        if (m_activeSceneIndex == -1) {
            m_activeSceneIndex = 0;
        }
        return m_scenes.size() - 1;
    }
    Scene* SceneManager::getActiveScene() {
        if (m_activeSceneIndex == -1) {
            fixActiveScene();
            return &m_scenes[m_activeSceneIndex];
        }
        return &m_scenes[m_activeSceneIndex];
    }
    void SceneManager::setActiveScene(int index) {
        if (index < 0 || index >= m_scenes.size()) {
            throw std::out_of_range("SceneManager::setActiveScene index out of range");
        }

        m_activeSceneIndex = index;
    }

    void SceneManager::removeScene(int index) {
        if (index < 0 || index >= m_scenes.size()) {
            throw std::out_of_range("SceneManager::removeScene index out of range");
        }

        Scene* activeScene = getActiveScene();
        Scene* sceneToDelete = &m_scenes[index];

        if (activeScene == sceneToDelete) {
            m_scenes.erase(m_scenes.begin() + index);
        } else {
            m_scenes.erase(m_scenes.begin() + index);
        }
        fixActiveScene();
    }

    void SceneManager::fixActiveScene() {
        if (m_activeSceneIndex == -1) {
            if (m_scenes.empty()) {
                m_activeSceneIndex = addScene(buildScene());
            } else {
                m_activeSceneIndex = 0;
            }
        }
    }

    Scene* SceneManager::getScene(int index) {
        if (index < 0 || index >= m_scenes.size()) {
            throw std::out_of_range("SceneManager::getScene index out of range");
        }
        return &m_scenes[index];
    }

    // Temp function until SceneFactory will implemented
    Scene SceneManager::buildScene() {
        Scene scene;
        Lighting::Light light(Lighting::LightType::POINT,
            Transform::Transform(
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)),
                glm::vec3(0.0f, 0.0f, 0.0f)
            )
            );
        light.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
        scene.addLight(light);
        return scene;
    }

}
