#include "Scene.h"

#include "camera/Camera.h"

namespace Scene {
    Scene::Scene() = default;
    Scene::~Scene() = default;

    void Scene::setGameObjects(std::vector<GameObject::GameObject>& gameObjects) {
        m_gameObjects = gameObjects;
    }

    std::vector<GameObject::GameObject>& Scene::getGameObjects() {
        return m_gameObjects;
    }
    void Scene::addGameObject(GameObject::GameObject& gameObject) {
        m_gameObjects.push_back(gameObject);
    }

    Camera::Camera* Scene::getActiveCamera() {
        return m_activeCamera;
    }

    void Scene::setActiveCamera(Camera::Camera* activeCamera) {
        m_activeCamera = activeCamera;
    }
}
