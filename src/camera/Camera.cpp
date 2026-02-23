#include "Camera.h"

#include <iostream>
#include <ostream>

namespace Camera {
    Camera::Camera(int width, int height, float fov, float zFar, float zNear) {
        m_width = width;
        m_height = height;
        m_fov = fov;
        m_zFar = zFar;
        m_zNear = zNear;

        Transform::Transform transform;
        m_transform = transform;

        m_projection = glm::perspective(glm::radians(m_fov), (float)m_width/(float)m_height, m_zNear, m_zFar);
        m_view = glm::mat4(1.0f);

        calculate();
        updateViewMatrix();
    }

    Camera::Camera(int width, int height, float fov, float zFar, float zNear, const Transform::Transform transform) {
        m_width = width;
        m_height = height;
        m_fov = fov;
        m_zFar = zFar;
        m_zNear = zNear;
        m_transform = transform;

        m_projection = glm::perspective(glm::radians(m_fov), (float)m_width/(float)m_height, m_zNear, m_zFar);
        m_view = glm::mat4(1.0f);

        calculate();
        updateViewMatrix();
    }

    Camera::~Camera() = default;

    void Camera::updateViewMatrix() {
        m_view = glm::lookAt(m_transform.getPosition(), m_transform.getPosition() + m_forward, m_up);
    }

    void Camera::calculate()
    {
        float pitch = glm::radians(m_transform.getRotation().x);
        float yaw = glm::radians(m_transform.getRotation().y);

        // Forward
        m_forward.x = cos(pitch) * sin(yaw);
        m_forward.y = sin(pitch);
        m_forward.z = cos(pitch) * cos(yaw);
        m_forward = glm::normalize(m_forward);

        // World up
        glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

        // Right and up
        m_right = glm::normalize(glm::cross(m_forward, worldUp));
        m_up = glm::normalize(glm::cross(m_right, m_forward));
    }

    void Camera::moveForward(float amount) {
        m_transform.setPosition(m_transform.getPosition() + m_forward * amount);
        updateViewMatrix();
    }
    void Camera::moveRight(float amount) {
        m_transform.setPosition(m_transform.getPosition() + m_right * amount);
        updateViewMatrix();
    }
    Transform::Transform Camera::getTransform() {
        return m_transform;
    }
    void Camera::setTransform(Transform::Transform transform) {
        m_transform = transform;
    }

    glm::mat4 Camera::getProjectionMatrix() {
        return m_projection;
    }
    glm::mat4 Camera::getViewMatrix() {
        return m_view;
    }

}
