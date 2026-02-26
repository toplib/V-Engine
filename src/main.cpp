#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "test.h"
#include "shader/Shader.h"
#include "shader/ShaderType.h"
#include "shader/ShaderProgram.h"
#include "window/Window.h"
#include "texture/Texture.h"
#include "camera/Camera.h"
#include "parser/obj2mesh/OBJ2MeshParser.h"
#include "debug/Logger.h"
#include "scene/Scene.h"
#include "rendering/Renderer.h"
#include "rendering/MeshRenderer.h"
#include "gameobject/GameObject.h"
#include "core/Transform.h"

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

const float MOUSE_SENSITIVITY = 0.2f;
const float MAX_PITCH = 80.0f;
const float MIN_PITCH = -80.0f;

double lastMouseX = 0.0;
double lastMouseY = 0.0;
bool isRMBPressed = false;
float currentPitch = 0.0f;
float currentYaw = 0.0f;

int main()
{
    Window::Window window(SCR_WIDTH, SCR_HEIGHT, "V-Engine");

    if (!window.isInitialized()) {
        std::cerr << "Failed to initialize window" << std::endl;
        return -1;
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

    Texture::Texture texture;
    if (!texture.load("/home/toplib/V-Engine/res/hazmat.jpg")) {
        std::cerr << "Failed to load texture" << std::endl;
    }

    // Parse mesh and upload to GPU
    Parser::OBJ2MeshParser parser;
    std::string source = loadShaderFromPath("/home/toplib/V-Engine/res/hazmat.obj"); // TODO: Implement normal assets system
    parser.source(&source);
    Mesh::Mesh mesh = parser.parse();
    mesh.build();

    Material::Material material;
    material.setShader(&shaderProgram);

    Rendering::MeshRenderer meshRenderer;
    meshRenderer.setMesh(mesh);
    meshRenderer.setMaterial(material);

    GameObject::GameObject gameObject;
    gameObject.setMeshRenderer(meshRenderer);
    gameObject.setTransform(Transform::Transform(
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f)),
        glm::vec3(1.0f)
    ));

    Scene::Scene scene;
    scene.addGameObject(gameObject);

    Debug::Logger logger("main");

    const int viewLoc = shaderProgram.getUniformLocation("view");
    const int projectionLoc = shaderProgram.getUniformLocation("projection");
    const int textureLoc = shaderProgram.getUniformLocation("ourTexture");

    shaderProgram.bind();
    if (textureLoc != -1) {
        shaderProgram.setUniform1i(textureLoc, 0);
    }

    Rendering::Renderer renderer(scene);

    Camera::Camera camera(SCR_WIDTH, SCR_HEIGHT, 40.0f, 100.0f, 0.01f,
        {
            {0.0f, 0.0f, -5.0f},
            glm::quat(glm::vec3(0.0f)),
            {1.0, 1.0f, 1.0f}
        });

    // Render loop
    glEnable(GL_DEPTH_TEST);

    while (!window.shouldClose()) {
        if (window.getKey(GLFW_KEY_ESCAPE) == Input::InputType::PRESS) {
            window.setShouldClose(true);
        }

        // TODO: Move to camera control class
        if (window.getKey(GLFW_KEY_W) == Input::InputType::PRESS) {
            camera.moveForward(0.04f);
        }
        if (window.getKey(GLFW_KEY_S) == Input::InputType::PRESS) {
            camera.moveForward(-0.04f);
        }
        if (window.getKey(GLFW_KEY_A) == Input::InputType::PRESS) {
            camera.moveRight(-0.04f);
        }
        if (window.getKey(GLFW_KEY_D) == Input::InputType::PRESS) {
            camera.moveRight(0.04f);
        }

        if (window.getMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == Input::InputType::PRESS) {
            if (!isRMBPressed) {
                isRMBPressed = true;
                window.setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                glfwGetCursorPos(window.getGLFWWindow(), &lastMouseX, &lastMouseY);
            }

            double xPos, yPos;
            glfwGetCursorPos(window.getGLFWWindow(), &xPos, &yPos);

            double deltaX = xPos - lastMouseX;
            double deltaY = yPos - lastMouseY;
            lastMouseX = xPos;
            lastMouseY = yPos;

            currentYaw   -= (float)deltaX * MOUSE_SENSITIVITY;
            currentPitch += (float)deltaY * MOUSE_SENSITIVITY;
            currentPitch  = glm::clamp(currentPitch, MIN_PITCH, MAX_PITCH);

            glm::quat yawQuat   = glm::angleAxis(glm::radians(currentYaw),   glm::vec3(0.0f, 1.0f, 0.0f));
            glm::quat pitchQuat = glm::angleAxis(glm::radians(currentPitch),  glm::vec3(1.0f, 0.0f, 0.0f));

            Transform::Transform camTransform = camera.getTransform();
            camTransform.setRotation(glm::normalize(yawQuat * pitchQuat));
            camera.setTransform(camTransform);
        } else {
            if (isRMBPressed) {
                isRMBPressed = false;
                window.setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.bind();

        if (viewLoc != -1) shaderProgram.setUniformMatrix4(viewLoc, camera.getViewMatrix());
        if (projectionLoc != -1) shaderProgram.setUniformMatrix4(projectionLoc, camera.getProjectionMatrix());
        texture.bind();
        renderer.render();

        window.swapBuffers();
        window.pollEvents();
    }

    // Cleanup
    texture.cleanup();

    window.shutdown();
    return 0;
}
