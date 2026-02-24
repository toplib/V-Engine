#include "Material.h"

namespace Material {
    Material::Material() = default;
    Material::~Material() = default;

    void Material::setShader(Shader::ShaderProgram* shaderProgram) {
        m_shaderProgram = shaderProgram;
    }

    Shader::ShaderProgram* Material::getShader() {
        return m_shaderProgram;
    }

    void Material::bind() const {
        if (m_shaderProgram) {
            m_shaderProgram->bind();
        }
    }
    void Material::unbind() const {
        if (m_shaderProgram) {
            m_shaderProgram->unbind();
        }
    }
}
