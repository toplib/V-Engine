#pragma once
#include "core/Transform.h"
#include "LightType.h"

namespace Lightning {
    class Light {
        public:
        Light(LightType type, Transform::Transform transform);
        ~Light();

        private:
        LightType m_type;
        Transform::Transform m_transform;
        glm::vec3 m_color;

    };
}
