#pragma once
#include "core/Transform.h"
#include "rendering/MeshRenderer.h"

namespace GameObject {
    class GameObject {
    public:
        GameObject();
        ~GameObject();

        Rendering::MeshRenderer getMeshRenderer();
        Transform::Transform getTransform();

        void setMeshRenderer(Rendering::MeshRenderer renderer);
        void setTransform(Transform::Transform transform);

        bool m_rendererEnabled = true;
    private:
        Rendering::MeshRenderer m_renderer;
        Transform::Transform m_transform;
    };
}
