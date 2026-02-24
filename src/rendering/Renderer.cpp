#include "Renderer.h"

namespace Rendering {
    Renderer::Renderer(Scene::Scene scene) {
        m_scene = scene;
    }
    Renderer::~Renderer() = default;

    void Renderer::render() {
        for (int i = 0; i < m_scene.getGameObjects().size(); i++) {
            if (m_scene.getGameObjects()[i].m_rendererEnabled != false) {
                MeshRenderer renderer = m_scene.getGameObjects()[i].getMeshRenderer();
                Mesh::Mesh mesh = renderer.getMesh();
                Material::Material material = renderer.getMaterial();
                material.getShader()->bind();
                mesh.bind();

                glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh.getVertices().size()));

                mesh.unbind();
                material.unbind();
            }
        }
    }
}