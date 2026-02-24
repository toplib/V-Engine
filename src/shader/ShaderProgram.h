#pragma once

#include "shader/Shader.h"
#include <string>
#include <unordered_map>

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Shader {

    class ShaderProgram {
    public:
        ShaderProgram();
        ~ShaderProgram();

        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        ShaderProgram(ShaderProgram&& other) noexcept;
        ShaderProgram& operator=(ShaderProgram&& other) noexcept;

        void attach(const Shader& shader);
        bool link();
        void bind() const;
        void unbind() const;

        // Returns -1 if the uniform is not found.
        int getUniformLocation(const std::string& name);

        void setUniformMatrix4(int location, const glm::mat4& matrix);
        void setUniformMatrix4(const std::string& name, const glm::mat4& matrix);

        void setUniform1i(int location, int value);
        void setUniform1i(const std::string& name, int value);

        unsigned int id() const { return m_id; }

    private:
        unsigned int m_id{};
        std::unordered_map<std::string, int> m_uniformLocationCache{};
    };
}
