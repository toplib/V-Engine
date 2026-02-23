#pragma once
#include "glm/glm.hpp"

namespace Transform {
    class Transform {
    public:
        Transform();
        Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
        ~Transform();

        glm::mat4 getModelMatrix();

        glm::vec3 getPosition();
        glm::vec3 getRotation();
        glm::vec3 getScale();

        void setPosition(glm::vec3 postion);
        void setRotation(glm::vec3 rotation);
        void setScale(glm::vec3 scale);
    private:
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;
    };
}