#pragma once
#include <vector>

#include "gameobject/GameObject.h"
#include "camera/Camera.h"
namespace Scene {
    class Scene {
    public:
        Scene();
        ~Scene();

        void setGameObjects(std::vector<GameObject::GameObject>& gameObjects);

        std::vector<GameObject::GameObject>& getGameObjects();
        void addGameObject(GameObject::GameObject& gameObject);

        Camera::Camera* getActiveCamera();
        void setActiveCamera(Camera::Camera* camera);
    private:
        Camera::Camera* m_activeCamera;
        std::vector<GameObject::GameObject> m_gameObjects;
    };
}
