#pragma once
#include "shader/ShaderProgram.h"
#include "texture/Texture.h"

namespace Material {
    class Material {
    public:
        Material();
        ~Material();

        void setShader(Shader::ShaderProgram* shaderProgram);
        void setTexture(Texture::Texture* texture);
        void setColor(glm::vec4 color);

        Shader::ShaderProgram* getShader();
        Texture::Texture* getTexture();
        glm::vec4 getColor();

        bool hasTexture() {
            return m_hasTexture;
        }

        void bind() const;
        void unbind() const;

    private:
        Shader::ShaderProgram* m_shaderProgram = nullptr;
        Texture::Texture* m_texture = nullptr;
        bool m_hasTexture = false;
        glm::vec4 m_color = glm::vec4(1.0f);
    };
}
