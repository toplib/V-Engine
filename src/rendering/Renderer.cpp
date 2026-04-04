#include <glad/glad.h>
#include "Renderer.h"
#include "../component/MeshRenderer.h"

namespace Rendering {
    Renderer::Renderer(Scene::Scene& scene) : m_scene(scene) {}
    Renderer::~Renderer() = default;

    void Renderer::render() {
        std::vector<GameObject::GameObject>& objects = m_scene.getGameObjects();
        Camera::Camera* activeCamera = m_scene.getActiveCamera();

        for (int i = 0; i < objects.size(); i++) {
            if (objects[i].m_rendererEnabled) {
                Component::MeshRenderer& mr = objects[i].getComponents();
                for (Component::Component component : objects[i].getComponents()) {
                    if (mr* = dynamic_cast<Component::MeshRenderer*>(component)) {

                    }
                }
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

                for (int i = 0; i < m_scene.m_lightsCount; i++) {
                    material.getShader()->setUniform1i("lights[" + std::to_string(i) + "].type", 0);
                    glm::vec3 p = m_scene.getLight(i)->getTransform().getPosition();
                    material.getShader()->setUniform4f(
                        "lights[" + std::to_string(i) + "].position",
                        glm::vec4(p.x, p.y, p.z, 1.0f)
                    );
                    glm::vec3 c = m_scene.getLight(i)->getColor();
                    material.getShader()->setUniform4f(
                        "lights[" + std::to_string(i) + "].color",
                        glm::vec4(c.x, c.y, c.z, 1.0f)
                    );
                    material.getShader()->setUniform1f("lights[" + std::to_string(i) + "].constant", m_scene.getLight(i)->getConstant());
                    material.getShader()->setUniform1f("lights[" + std::to_string(i) + "].linear", m_scene.getLight(i)->getLinear());
                    material.getShader()->setUniform1f("lights[" + std::to_string(i) + "].quadratic", m_scene.getLight(i)->getQuadratic());
                }

                glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh.getVertices().size()));

                mesh.unbind();
                material.unbind();
            }
        }
    }
}
