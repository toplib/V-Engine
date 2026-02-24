#include "Scene.h"

namespace Scene {
    Scene::Scene() = default;
    Scene::~Scene() = default;

    void Scene::setGameObjects(std::vector<GameObject::GameObject>& gameObjects) {
        m_gameObjects = gameObjects;
    }

    std::vector<GameObject::GameObject> Scene::getGameObjects() {
        return m_gameObjects;
    }
}
