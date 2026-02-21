#include <glad/glad.h>
#include "shader/Shader.h"
#include <stdexcept>
#include <vector>
#include <iostream>

namespace Shader {

    Shader::Shader(ShaderType type) : m_type(type) {
        m_id = glCreateShader(
            type == ShaderType::VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER
        );
        if (!m_id) {
            std::cerr << "Failed to create shader object" << std::endl;
            throw std::runtime_error("Failed to create shader object");
        }
    }

    Shader::~Shader() {
        if (m_id) {
            glDeleteShader(m_id);
        }
    }

    Shader::Shader(Shader&& other) noexcept : m_id(other.m_id), m_type(other.m_type) {
        other.m_id = 0;
    }

    Shader& Shader::operator=(Shader&& other) noexcept {
        if (this != &other) {
            if (m_id) {
                glDeleteShader(m_id);
            }
            m_id = other.m_id;
            m_type = other.m_type;
            other.m_id = 0;
        }
        return *this;
    }

    void Shader::source(const char* source) {
        glShaderSource(m_id, 1, &source, nullptr);
    }

    bool Shader::compile() {
        glCompileShader(m_id);

        int success;
        glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
        if (!success) {
            std::vector<char> infoLog(512);
            glGetShaderInfoLog(m_id, infoLog.size(), nullptr, infoLog.data());
            std::cerr << "Shader compilation failed:" << std::endl;
            std::cerr << infoLog.data() << std::endl;
            throw std::runtime_error(
                std::string("Shader compilation failed: ") + infoLog.data()
            );
        }
        return true;
    }
}
