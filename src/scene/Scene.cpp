#include "Scene.h"

#include "camera/Camera.h"
#include "debug/Logger.h"

namespace Scene {
    Scene::Scene() : m_logger(Debug::Logger("Scene")) {

    }

    Scene::~Scene() = default;


    void Scene::setGameObjects(const std::vector<GameObject::GameObject>* gameObjects) {
        if (gameObjects == nullptr) {
            m_logger.printWarning("Scene::setGameObjects: gameObjects is nullptr skipping function call");
            return;
        }
        m_gameObjects = *gameObjects;
    }

    std::vector<GameObject::GameObject>& Scene::getGameObjects() {
        return m_gameObjects;
    }
    void Scene::addGameObject(GameObject::GameObject* gameObject) {
        if (gameObject == nullptr) {
            m_logger.printWarning("Scene::addGameObject: gameObject is nullptr skipping function call");
            return;
        }
        m_gameObjects.push_back(*gameObject);
    }

    Lighting::Light* Scene::getLights() {
        return m_lights;
    }
    void Scene::addLight(Lighting::Light& light) {
        if (m_lightsCount < m_maxLights) {
            int index = (m_startChangeLights + m_lightsCount) % m_maxLights;
            m_lights[index] = light;
            ++m_lightsCount;
        } else {
            m_lights[m_startChangeLights] = light;
            m_startChangeLights = (m_startChangeLights + 1) % m_maxLights;
        }
    }
    void Scene::setLights(Lighting::Light* lights, int nLights) {
        if (lights == nullptr) return;

        for(int i = 0; i < m_maxLights && i <= nLights; i++) {
            m_lights[i] = lights[i];
        }
    }

    Lighting::Light *Scene::getLight(unsigned int index) {
        if (index >= m_maxLights || index >= m_lightsCount) {
            return nullptr;
        }
        return &m_lights[index];
    }

    Camera::Camera* Scene::getActiveCamera() {
        return m_activeCamera;
    }

    void Scene::setActiveCamera(Camera::Camera* activeCamera) {
        m_activeCamera = activeCamera;
    }
}
