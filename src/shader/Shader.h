#pragma once

#include "shader/ShaderType.h"

namespace Shader {
    class Shader {
    public:
        Shader(ShaderType type);
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        Shader(Shader&& other) noexcept;
        Shader& operator=(Shader&& other) noexcept;

        void source(const char* source);
        bool compile();
        unsigned int id() const { return m_id; }

    private:
        unsigned int m_id;
        ShaderType m_type;
    };
}
