#pragma once
#include "shader/ShaderProgram.h"

namespace Material {
    class Material {
    public:
        Material();
        ~Material();

        void setShader(Shader::ShaderProgram* shaderProgram);

        Shader::ShaderProgram* getShader();
        void bind() const;
        void unbind() const;

    private:
        Shader::ShaderProgram* m_shaderProgram = nullptr;
    };
}
