#pragma once
#include "glm/glm.hpp"

namespace Transform {
    class Transform {
    public:
        Transform();
        Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
        ~Transform();

        glm::mat4 getModelMatrix();
    private:
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;
    };
}