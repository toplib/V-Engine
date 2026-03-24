#pragma once
#include <vector>

#include "gameobject/GameObject.h"
#include "camera/Camera.h"
#include "lightning/Light.h"

#define MAX_LIGHTS 10
namespace Scene {
    class Scene {
    public:
        Scene();
        ~Scene();

        void setGameObjects(std::vector<GameObject::GameObject>& gameObjects);

        std::vector<GameObject::GameObject>& getGameObjects();
        void addGameObject(GameObject::GameObject& gameObject);

        Lightning::Light* getLights();
        void setLights(Lightning::Light* lights, int nLights);
        void addLight(Lightning::Light& light);

        Camera::Camera* getActiveCamera();
        void setActiveCamera(Camera::Camera* camera);
    private:
        Camera::Camera* m_activeCamera;
        std::vector<GameObject::GameObject> m_gameObjects;

        Lightning::Light m_lights[MAX_LIGHTS];
        int m_lightsCount;
        int m_maxLights = MAX_LIGHTS;
    };
}
