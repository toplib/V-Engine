#pragma once
#include "core/Transform.h"
#include "LightType.h"

namespace Lightning {
    class Light {
        public:
        virtual ~Light();

        virtual LightType getLightType();
        virtual Transform::Transform getTransform();
        virtual glm::vec3 getColor();
        virtual float getIntensity();
    };
}
