#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "test.h"
#include "shader/Shader.h"
#include "shader/ShaderType.h"
#include "shader/ShaderProgram.h"
#include "window/Window.h"
#include "mesh/Mesh.h"
#include "texture/Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;



int main()
{
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

    Window::Window window(SCR_WIDTH, SCR_HEIGHT, "V-Engine");

    if (!window.isInitialized()) {
        std::cerr << "Failed to initialize window" << std::endl;
        return -1;
    }

    // Create mesh with vertex data
    std::vector<Mesh::Vertex> vertices = {
        {{ 0.5f,  0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},   // top right
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},   // bottom right
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},   // bottom left
        {{-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}    // top left
    };

    std::vector<unsigned int> indices = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };

    Mesh::Mesh mesh;
    mesh.setVertices(vertices);
    mesh.setIndices(indices);
    mesh.build();

    // Load texture
    Texture::Texture texture;
    if (!texture.load("/home/toplib/V-Engine/res/texture.jpg")) {
        std::cerr << "Failed to load texture" << std::endl;
    }

    // Shaders
    Shader::Shader vertexShader(Shader::ShaderType::VERTEX);
    Shader::Shader fragmentShader(Shader::ShaderType::FRAGMENT);

    vertexShader.source(loadVertexShader());
    fragmentShader.source(loadFragmentShader());

    try {
        if (!vertexShader.compile() || !fragmentShader.compile()) {
            std::cerr << "Cannot compile shaders" << std::endl;
            return -1;
        }
    } catch (char* str) {
        std::cerr << "Shader compilation exception: " << str << std::endl;
    }

    Shader::ShaderProgram shaderProgram;
    shaderProgram.attach(vertexShader);
    shaderProgram.attach(fragmentShader);

    try {
        if (!shaderProgram.link()) {
            std::cerr << "Cannot link shader program" << std::endl;
            return -1;
        }
    } catch (char* str) {
        std::cerr << "Shader linking exception: " << str << std::endl;
    }

    unsigned int transformLoc = shaderProgram.getUniformLocation("transform");
    shaderProgram.setUnformMatrix4(transformLoc, trans);
    // Render loop
    while (!window.shouldClose()) {
        if (window.getKey(GLFW_KEY_ESCAPE) == Input::InputType::PRESS) {
            window.setShouldClose(true);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        texture.bind();
        shaderProgram.use();
        mesh.bind();
        mesh.draw();
        mesh.unbind();

        window.swapBuffers();
        window.pollEvents();
    }

    // Cleanup
    mesh.cleanup();
    texture.cleanup();

    window.shutdown();
    return 0;
}
