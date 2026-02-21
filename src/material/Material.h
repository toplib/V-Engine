#pragma once
#include "shader/ShaderProgram.h"

namespace Material {
    class Material {
    public:
        Material();
        ~Material();

        void setShader(Shader::ShaderProgram* shaderProgram);
        void use() const;

    private:
        Shader::ShaderProgram* m_shaderProgram = nullptr;
    };
}
