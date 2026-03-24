#pragma once
#include <vector>

#include "gameobject/GameObject.h"
#include "camera/Camera.h"
#include "lighting/Light.h"
#include "debug/Logger.h"
#define MAX_LIGHTS 10
namespace Scene {
    class Scene {
    public:
        Scene();
        ~Scene();

        void setGameObjects(const std::vector<GameObject::GameObject>* gameObjects);

        std::vector<GameObject::GameObject>& getGameObjects();
        void addGameObject(GameObject::GameObject* gameObject);

        Lighting::Light* getLights();
        void setLights(Lighting::Light* lights, int nLights);
        void addLight(Lighting::Light& light);
        Lighting::Light *getLight(unsigned int index);

        Camera::Camera* getActiveCamera();
        void setActiveCamera(Camera::Camera* camera);
        int m_lightsCount = 0;
    private:
        Debug::Logger m_logger;
        Camera::Camera* m_activeCamera = nullptr;
        std::vector<GameObject::GameObject> m_gameObjects;

        Lighting::Light m_lights[MAX_LIGHTS];
        int m_startChangeLights = 0;
        int m_maxLights = MAX_LIGHTS;
    };
}
