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

    unsigned int ShaderProgram::getUniformLocation(std::string name) {
        return glGetUniformLocation(m_id, name.c_str());
    }
    void ShaderProgram::setUnformMatrix4(unsigned int transformLoc, glm::mat4 matrix) {
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}
