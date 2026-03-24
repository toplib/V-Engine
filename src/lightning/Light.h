#pragma once
#include "core/Transform.h"
#include "LightType.h"

namespace Lightning {
    class Light {
        public:
        Light();
        Light(LightType type, Transform::Transform transform);
        ~Light();

        LightType getLightType();
        void setLightType(LightType type);
        Transform::Transform getTransform();
        void setTransform(Transform::Transform transform);
        glm::vec3 getColor();
        void setColor(glm::vec3 color);
        private:
        LightType m_type;
        Transform::Transform m_transform;
        glm::vec3 m_color;

    };
}
