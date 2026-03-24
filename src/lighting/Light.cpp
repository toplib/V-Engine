#include "Light.h"

namespace Lighting {
    Light::Light() {
        m_type = LightType::POINT;
        m_transform = Transform::Transform(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)),
            glm::vec3(0.0f, 0.0f, 0.0f)
        );
    }
    Light::Light(LightType type, Transform::Transform transform) {
        m_type = type;
        m_transform = transform;
    }
    Light::~Light() = default;


    LightType Light::getLightType() {
        return m_type;
    }
    void Light::setLightType(LightType type) {
        m_type = type;
    }

    Transform::Transform Light::getTransform() {
        return m_transform;
    }

    void Light::setTransform(Transform::Transform transform) {
        m_transform = transform;
    }
    glm::vec3 Light::getColor() {
        return m_color;
    }

    void Light::setColor(glm::vec3 color) {
        m_color = color;
    }
}
