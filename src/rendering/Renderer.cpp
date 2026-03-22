#include <glad/glad.h>
#include "Renderer.h"
#include "MeshRenderer.h"

namespace Rendering {
    Renderer::Renderer(Scene::Scene& scene) : m_scene(scene) {}
    Renderer::~Renderer() = default;

    void Renderer::render() {
        std::vector<GameObject::GameObject>& objects = m_scene.getGameObjects();
        Camera::Camera* activeCamera = m_scene.getActiveCamera();

        for (int i = 0; i < objects.size(); i++) {
            if (objects[i].m_rendererEnabled) {
                MeshRenderer& mr = objects[i].getMeshRenderer();
                Mesh::Mesh& mesh = mr.getMesh();
                Material::Material& material = mr.getMaterial();

                if (!mesh.isBuilt()) {
                    mesh.build();
                }

                material.bind();

                // Apply per-object transform
                if (material.getShader()) {
                    material.getShader()->setUniformMatrix4(
                        "model",
                        objects[i].getTransform().getModelMatrix()
                    );
                    material.getShader()->setUniformMatrix4(
                        "projection",
                        activeCamera->getProjectionMatrix()
                        );
                    material.getShader()->setUniformMatrix4(
                        "view",
                        activeCamera->getViewMatrix()
                        );
                    material.getShader()->setUniform3f(
                        "viewPos",
                        activeCamera->getTransform().getPosition()
                        );
                }

                mesh.bind();

                glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh.getVertices().size()));

                mesh.unbind();
                material.unbind();
            }
        }
    }
}
