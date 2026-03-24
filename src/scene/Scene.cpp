#include "Scene.h"

#include "camera/Camera.h"
#include "debug/Logger.h"

namespace Scene {
    Scene::Scene() = default;
    Scene::~Scene() = default;


    void Scene::setGameObjects(const std::vector<GameObject::GameObject>* gameObjects) {
        if (gameObjects == nullptr) ;
        m_gameObjects = *gameObjects;
    }

    std::vector<GameObject::GameObject>& Scene::getGameObjects() {
        return m_gameObjects;
    }
    void Scene::addGameObject(GameObject::GameObject& gameObject) {
        m_gameObjects.push_back(gameObject);
    }

    Lightning::Light* Scene::getLights() {
        return m_lights;
    }
    void Scene::addLight(Lightning::Light& light) {
        if (m_lightsCount < m_maxLights) {
            m_lightsCount = 1;
        }
        m_lights[m_lightsCount] = light;
    }
    void Scene::setLights(Lightning::Light* lights, int nLights) {
        if (lights == nullptr) return;

        for(int i = 0; i < m_maxLights && i <= nLights; i++) {
            m_lights[i] = lights[i];
        }
    }

    Camera::Camera* Scene::getActiveCamera() {
        return m_activeCamera;
    }

    void Scene::setActiveCamera(Camera::Camera* activeCamera) {
        m_activeCamera = activeCamera;
    }
}
