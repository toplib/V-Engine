#include "GameObject.h"

#include "component/Component.h"
#include <iostream>
#include <memory>

namespace GameObject {
    GameObject::GameObject() = default;
    GameObject::~GameObject() = default;


    std::vector<Component::Component> GameObject::getComponents() {
        return m_components;
    }
    void GameObject::setComponents(std::vector<Component::Component> components) {
        m_components = components;
    }

    template<typename T, typename... Args>
    T* GameObject::addComponent(Args&&... args) {
        auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
        T* raw = ptr.get();
        m_components.push_back(std::move(ptr));
        return raw;
    }


    void GameObject::setTransform(Transform::Transform transform) {
        m_transform = transform;
    }

    Transform::Transform& GameObject::getTransform() {
        return m_transform;
    }

    void GameObject::start() {
        for (Component::Component component : m_components) {
            component.start();
        }
        // do nothing at future we will execute this method's in the components
    }

    void GameObject::update() {
        // do nothing
    }
}
