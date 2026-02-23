#pragma once
#include "core/Transform.h"
#include "glm/fwd.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Camera {
    class Camera {
        public:
        Camera(int width, int height, float fov, float zFar, float zNear);
        Camera(int width, int height, float fov, float zFar, float zNear, const Transform::Transform transform);
        ~Camera();

        void moveForward(float amount);
        void moveRight(float amount);

        Transform::Transform getTransform();
        void setTransform(Transform::Transform transform);

        glm::mat4 getProjectionMatrix();
        glm::mat4 getViewMatrix();

        private:
        int m_width;
        int m_height;

        float m_fov;
        float m_zFar;
        float m_zNear;

        Transform::Transform m_transform;
        glm::mat4 m_view;
        glm::mat4 m_projection;

        glm::vec3 m_globalUp;
        glm::vec3 m_forward;
        glm::vec3 m_right;
        glm::vec3 m_up;

        void updateViewMatrix();
        void calculate();
    };
}
