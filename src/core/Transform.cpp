#include "Transform.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

namespace Transform {
    Transform::Transform() {
        m_position = glm::vec3(0.0f, 0.0f, 0.0f);
        m_rotation = glm::quat(glm::vec3(0.0f));
        m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    Transform::Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale) {
        m_position = position;
        m_rotation = rotation;
        m_scale = scale;
    }

    Transform::~Transform() = default;

    glm::mat4 Transform::getModelMatrix() {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_position);
        model = model * glm::toMat4(m_rotation);
        model = glm::scale(model, m_scale);
        return model;
    }

    glm::vec3 Transform::getPosition() {
        return m_position;
    }
    glm::quat Transform::getRotation() {
        return m_rotation;
    }
    glm::vec3 Transform::getScale() {
        return m_scale;
    }


    void Transform::setPosition(glm::vec3 position) {
        m_position = position;
    }
    void Transform::setRotation(glm::quat rotation) {
        m_rotation = rotation;
    }
    void Transform::setScale(glm::vec3 scale) {
        m_scale = scale;
    }
}
