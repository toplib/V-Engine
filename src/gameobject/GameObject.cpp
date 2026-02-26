#include "GameObject.h"

namespace GameObject {
    GameObject::GameObject() = default;
    GameObject::~GameObject() = default;

    void GameObject::setTransform(Transform::Transform transform) {
        m_transform = transform;
    }

    void GameObject::setMeshRenderer(Rendering::MeshRenderer renderer) {
        m_renderer = renderer;
    }

    Rendering::MeshRenderer& GameObject::getMeshRenderer() {
        return m_renderer;
    }
    Transform::Transform& GameObject::getTransform() {
        return m_transform;
    }

    void GameObject::start() {
        // do nothing at future we will execute this method's in the components
    }

    void GameObject::update() {
        // do nothing
    }
}
