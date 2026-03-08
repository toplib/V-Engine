#include "Material.h"

#include "texture/Texture.h"

namespace Material {
    Material::Material() = default;
    Material::~Material() = default;

    void Material::setShader(Shader::ShaderProgram* shaderProgram) {
        m_shaderProgram = shaderProgram;
    }

    void Material::setTexture(Texture::Texture* texture) {
        if (texture != nullptr) {
            m_hasTexture = true;
            m_texture = texture;
        }
    }

    void Material::setColor(glm::vec4 color) {
        m_color = color;
    }

    Shader::ShaderProgram* Material::getShader() {
        return m_shaderProgram;
    }

    Texture::Texture* Material::getTexture() {
        return m_texture;
    }

    glm::vec4 Material::getColor() {
        return m_color;
    }

    void Material::bind() const {
        if (m_shaderProgram) {
            if (m_hasTexture) {
                m_texture->bind();
            }
            m_shaderProgram->bind();

            // Set all fields
            m_shaderProgram->setUniform1i("m_hasTexture", m_hasTexture);
            m_shaderProgram->setUniform4f("m_color", m_color);
        }
    }
    void Material::unbind() const {
        if (m_shaderProgram) {
            m_texture->unbind();
            m_shaderProgram->unbind();
        }
    }
}
