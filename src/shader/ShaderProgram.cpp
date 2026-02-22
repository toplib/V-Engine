#include <glad/glad.h>
#include "shader/ShaderProgram.h"
#include <stdexcept>
#include <vector>
#include <iostream>

namespace Shader {

    ShaderProgram::ShaderProgram() {
        m_id = glCreateProgram();
        if (!m_id) {
            std::cerr << "Failed to create program object" << std::endl;
            throw std::runtime_error("Failed to create program object");
        }
    }

    ShaderProgram::~ShaderProgram() {
        if (m_id) {
            glDeleteProgram(m_id);
        }
    }

    ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
        : m_id(other.m_id) {
        other.m_id = 0;
    }

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
        if (this != &other) {
            if (m_id) {
                glDeleteProgram(m_id);
            }
            m_id = other.m_id;
            other.m_id = 0;
        }
        return *this;
    }

    void ShaderProgram::attach(const Shader& shader) {
        glAttachShader(m_id, shader.id());
    }

    bool ShaderProgram::link() {
        glLinkProgram(m_id);

        int success;
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (!success) {
            std::vector<char> infoLog(512);
            glGetProgramInfoLog(m_id, infoLog.size(), nullptr, infoLog.data());
            std::cerr << "Program linking failed:" << std::endl;
            std::cerr << infoLog.data() << std::endl;
            throw std::runtime_error(
                std::string("Program linking failed: ") + infoLog.data()
            );
        }
        return true;
    }

    void ShaderProgram::use() const {
        glUseProgram(m_id);
    }

    int ShaderProgram::getUniformLocation(const std::string& name) {
        if (auto it = m_uniformLocationCache.find(name); it != m_uniformLocationCache.end()) {
            return it->second;
        }

        const int location = glGetUniformLocation(m_id, name.c_str());
        if (location == -1) {
            std::cerr << "Uniform '" << name << "' not found" << std::endl;
        }

        m_uniformLocationCache.emplace(name, location);
        return location;
    }

    void ShaderProgram::setUniformMatrix4(int location, const glm::mat4& matrix) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void ShaderProgram::setUniformMatrix4(const std::string& name, const glm::mat4& matrix) {
        const int location = getUniformLocation(name);
        if (location != -1) {
            setUniformMatrix4(location, matrix);
        }
    }

    void ShaderProgram::setUniform1i(int location, int value) {
        glUniform1i(location, value);
    }

    void ShaderProgram::setUniform1i(const std::string& name, int value) {
        const int location = getUniformLocation(name);
        if (location != -1) {
            setUniform1i(location, value);
        }
    }
}
