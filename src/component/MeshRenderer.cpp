#include "MeshRenderer.h"

namespace Component {
    MeshRenderer::MeshRenderer() = default;
    MeshRenderer::~MeshRenderer() = default;

    void MeshRenderer::start() {
        // Do nothing
    }

    void MeshRenderer::update() {
        // Do nothing
    }

    void MeshRenderer::setMaterial(Material::Material material) {
        m_material = material;
    }

    void MeshRenderer::setMesh(Mesh::Mesh mesh) {
        m_mesh = mesh;
    }

    Material::Material& MeshRenderer::getMaterial() {
        return m_material;
    }

    Mesh::Mesh& MeshRenderer::getMesh() {
        return m_mesh;
    }
}
