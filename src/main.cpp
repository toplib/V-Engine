#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "shader/Shader.h"
#include "shader/ShaderType.h"
#include "shader/ShaderProgram.h"
#include "window/Window.h"
#include "mesh/Mesh.h"
#include "texture/Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "test.h"
#include "camera/Camera.h"
#include "parser/obj2mesh/OBJ2MeshParser.h"
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

int main()
{
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    Parser::OBJ2MeshParser parser;
    std::string source = loadShaderFromPath("/home/toplib/V-Engine/res/cube.obj"); // TODO: Implement normal assets system
    parser.source(&source);

    parser.parse();

    Window::Window window(SCR_WIDTH, SCR_HEIGHT, "V-Engine");

    if (!window.isInitialized()) {
        std::cerr << "Failed to initialize window" << std::endl;
        return -1;
    }

    // Create mesh with vertex data
    std::vector<Mesh::Vertex> vertices = {
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},

        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},

        {{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},

        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},

        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},

        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}}
    };



    std::vector<unsigned int> indices = {
        0,  1,  2,  3,  4,  5,
        6,  7,  8,  9, 10, 11,
       12, 13, 14, 15, 16, 17,
       18, 19, 20, 21, 22, 23,
       24, 25, 26, 27, 28, 29,
       30, 31, 32, 33, 34, 35
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

    try {
        vertexShader.sourceFromFile("/home/toplib/V-Engine/vertex.glsl");
        fragmentShader.sourceFromFile("/home/toplib/V-Engine/fragment.glsl");

        vertexShader.compile();
        fragmentShader.compile();
    } catch (const std::exception& e) {
        std::cerr << "Shader compilation exception: " << e.what() << std::endl;
        return -1;
    }

    Shader::ShaderProgram shaderProgram;
    shaderProgram.attach(vertexShader);
    shaderProgram.attach(fragmentShader);

    try {
        shaderProgram.link();
    } catch (const std::exception& e) {
        std::cerr << "Shader linking exception: " << e.what() << std::endl;
        return -1;
    }

    const int modelLoc = shaderProgram.getUniformLocation("model");
    const int viewLoc = shaderProgram.getUniformLocation("view");
    const int projectionLoc = shaderProgram.getUniformLocation("projection");
    const int textureLoc = shaderProgram.getUniformLocation("ourTexture");

    shaderProgram.use();
    if (textureLoc != -1) {
        shaderProgram.setUniform1i(textureLoc, 0);
    }
    Camera::Camera camera(SCR_WIDTH, SCR_HEIGHT, 40.0f, 100.0f, 0.01f,{{0.0f, 0.0f, -5.0f}, {0.0f, 2.0f, 0.0f}, {1.0, 1.0f, 1.0f}});
    // Render loop
    while (!window.shouldClose()) {
        glEnable(GL_DEPTH_TEST);
        if (window.getKey(GLFW_KEY_ESCAPE) == Input::InputType::PRESS) {
            window.setShouldClose(true);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();

        if (modelLoc != -1) shaderProgram.setUniformMatrix4(modelLoc, model);
        if (viewLoc != -1) shaderProgram.setUniformMatrix4(viewLoc, camera.getViewMatrix());
        if (projectionLoc != -1) shaderProgram.setUniformMatrix4(projectionLoc, camera.getProjectionMatrix());
        texture.bind();
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
