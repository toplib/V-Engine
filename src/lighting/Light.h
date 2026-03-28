#pragma once
#include "core/Transform.h"
#include "LightType.h"

namespace Lighting {
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
        float getConstant();
        void setConstant(float constant);
        float getLinear();
        void setLinear(float linear);
        float getQuadratic();
        void setQuadratic(float quadratic);
        private:
        LightType m_type;
        Transform::Transform m_transform;
        glm::vec3 m_color;
        float m_constant;
        float m_linear;
        float m_quadratic;

    };
}
