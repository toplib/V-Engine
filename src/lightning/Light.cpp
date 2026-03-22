#include "Light.h"

namespace Lightning {
    Light::Light(LightType type, Transform::Transform transform) {
        m_type = type;
        m_transform = transform;
    }

}
