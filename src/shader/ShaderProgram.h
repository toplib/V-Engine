#pragma once

#include "shader/Shader.h"

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
        void use() const;

        unsigned int id() const { return m_id; }

    private:
        unsigned int m_id{};
    };
}
