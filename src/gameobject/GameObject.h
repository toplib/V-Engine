#pragma once
#include "core/Transform.h"
#include "component/Component.h"
namespace GameObject {
    class GameObject {
    public:
        GameObject();
        ~GameObject();

        std::vector<Component::Component> getComponents();
        void setComponents(std::vector<Component::Component> components);
        template<typename T, typename... Args>
        T* addComponent(Args&&... args);

        Transform::Transform& getTransform();
        void setTransform(Transform::Transform transform); 

        void start();
        void update();

        bool m_rendererEnabled = true;
    private:
        std::vector<Component::Component> m_components;
        Transform::Transform m_transform;
    };
}
