#include "Material.h"

namespace Material {
    Material::Material() = default;
    Material::~Material() = default;

    void Material::setShader(Shader::ShaderProgram* shaderProgram) {
        m_shaderProgram = shaderProgram;
    }

    void Material::use() const {
        if (m_shaderProgram) {
            m_shaderProgram->use();
        }
    }
}
