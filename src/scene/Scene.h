#pragma once
#include <vector>

#include "gameobject/GameObject.h"

namespace Scene {
    class Scene {
    public:
        Scene();
        ~Scene();

        void setGameObjects(std::vector<GameObject::GameObject>& gameObjects);

        std::vector<GameObject::GameObject> getGameObjects();
        void getGameObjects(std::vector<GameObject::GameObject>& gameObjects);
    private:
        std::vector<GameObject::GameObject> m_gameObjects;
    };
}
