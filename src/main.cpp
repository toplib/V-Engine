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
#include "debug/Logger.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


int SCR_WIDTH = 1280;
int SCR_HEIGHT = 720;

const float MOUSE_SENSITIVITY = 0.2f;
const float MAX_PITCH = 80.0f;
const float MIN_PITCH = -80.0f;

double lastMouseX = 0.0;
double lastMouseY = 0.0;
bool isRMBPressed = false;
float currentPitch = 0.0f;
float currentYaw = 0.0f;
Debug::Logger logger("main");
int main()
{
    Window::Window window(SCR_WIDTH, SCR_HEIGHT, "V-Engine");

    if (!window.isInitialized()) {
        logger.printError("Failed to initialize window");
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
    Texture::Texture texture1;
    if (!texture1.load("/home/toplib/V-Engine/res/texture.jpg")) { // "/home/toplib/V-Engine/res/SampleScene/hltex/C1A1_LINO.png"
        std::cerr << "Failed to load texture" << std::endl;
    }
    Texture::Texture texture2;
    if (!texture2.load("/home/toplib/V-Engine/res/SampleScene/pc.png")) { // "/home/toplib/V-Engine/res/SampleScene/hltex/C1A1_LINO.png"
        std::cerr << "Failed to load texture" << std::endl;
    }

    // Parse mesh and upload to GPU
    Parser::OBJ2MeshParser parser;
    std::string source = loadShaderFromPath("/home/toplib/V-Engine/res/hazmat.obj"); // TODO: Implement normal assets system
    parser.source(&source);
    Mesh::Mesh mesh = parser.parse();
    mesh.build();
    source = loadShaderFromPath("/home/toplib/V-Engine/res/SampleScene/plane.obj");
    parser.source(&source);
    Mesh::Mesh mesh1 = parser.parse();
    mesh1.build();
    source = loadShaderFromPath("/home/toplib/V-Engine/res/SampleScene/pc.obj");
    parser.source(&source);
    Mesh::Mesh mesh2 = parser.parse();
    mesh2.build();

    // Material
    Material::Material material;
    material.setShader(&shaderProgram);
    material.setTexture(&texture);
    Material::Material material1;
    material1.setShader(&shaderProgram);
    //material1.setColor(glm::vec4(245.0f / 255.0f, 141.0f / 255.0f, 66.0f / 255.0f, 1.0f));
    //material1.setColor(glm::vec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f));
    material1.setTexture(&texture1);
    Material::Material material2;
    material2.setShader(&shaderProgram);
    material2.setTexture(&texture2);

    Rendering::MeshRenderer meshRenderer;
    meshRenderer.setMesh(mesh);
    meshRenderer.setMaterial(material);
    Rendering::MeshRenderer meshRenderer1;
    meshRenderer1.setMesh(mesh1);
    meshRenderer1.setMaterial(material1);
    Rendering::MeshRenderer meshRenderer2;
    meshRenderer2.setMesh(mesh2);
    meshRenderer2.setMaterial(material2);

    GameObject::GameObject gameObject;
    gameObject.setMeshRenderer(meshRenderer);
    gameObject.setTransform(Transform::Transform(
        glm::vec3(0.0f, -1.0f, -1.0f),
        glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f)),
        glm::vec3(1.0f)
    ));
    GameObject::GameObject gameObject1;
    gameObject1.setMeshRenderer(meshRenderer1);
    gameObject1.setTransform(Transform::Transform(
        glm::vec3(0.0f, -1.0f, -1.0f),
        glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f)),
        glm::vec3(1.0f)
    ));
    GameObject::GameObject gameObject2;
    gameObject2.setMeshRenderer(meshRenderer2);
    gameObject2.setTransform(Transform::Transform(
        glm::vec3(0.0f,  1.0f, -2.0f),
        glm::quat(glm::vec3(0.0f, glm::radians(90.0f), 0.0f)),
        glm::vec3(0.2f)
    ));

    Scene::Scene scene;
    scene.addGameObject(&gameObject);
    scene.addGameObject(&gameObject1);
    scene.addGameObject(&gameObject2);

    Debug::Logger logger("main");

    Rendering::Renderer renderer(scene);

    Camera::Camera camera(SCR_WIDTH, SCR_HEIGHT, 40.0f, 100.0f, 0.01f,
        {
            {0.0f, 0.0f, -5.0f},
            glm::quat(glm::vec3(0.0f)),
            {1.0, 1.0f, 1.0f}
        });
    Camera::Camera camera1(SCR_WIDTH, SCR_HEIGHT, 40.0f, 100.0f, 0.01f,
        {
            {0.0f, 0.0f, -5.0f},
            glm::quat(glm::vec3(0.0f)),
            {1.0, 1.0f, 1.0f}
        });

    Lighting::Light light = Lighting::Light();
    light.setLightType(Lighting::LightType::POINT);
    light.setTransform(Transform::Transform());
    light.setColor(glm::vec3(1.0f, 1.0f, 1.0f));

    Lighting::Light light2 = Lighting::Light();
    light2.setLightType(Lighting::LightType::POINT);
    light2.setTransform(
        Transform::Transform(
        glm::vec3(-2.0f,  2.0f, -2.0f),
        glm::quat(glm::vec3(0.0f, glm::radians(90.0f), 0.0f)),
          glm::vec3(0.2f)
            )

    );
    light2.setColor(glm::vec3(1.0f, 1.0f, 1.0f));

    Lighting::Light light1 = Lighting::Light();
    light1.setLightType(Lighting::LightType::POINT);
    light1.setTransform(Transform::Transform(
         glm::vec3(0.0f, -5.0f, -1.0f),
         glm::quat(glm::vec3(1.0f)),
         glm::vec3(1.0f)
     ));
    light1.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
    scene.addLight(light);
    //scene.addLight(light2);
    scene.addLight(light1);

    scene.setActiveCamera(&camera);
    // Render loop
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // ImGUI init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.getGLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");


    // Debug menu parameters
    float moveSpeed = 0.04f;
    bool changeSpeedManually = false;
    bool lockFPS = false;
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Add Object Menu
    bool showAddObjects = false;
    GameObject::GameObject tempGameObject;
    tempGameObject.setMeshRenderer(meshRenderer);
    char objPath[256] = "";
    char texturePath[256] = "";

    std::vector<glm::vec3> objectEulerAngles;

    glm::vec3 tempPosition = glm::vec3(0.0f);
    glm::vec3 tempRotationEuler = glm::vec3(0.0f);
    glm::vec3 tempScale = glm::vec3(1.0f);

    while (!window.shouldClose()) {
        if (window.getKey(GLFW_KEY_ESCAPE) == Input::InputType::PRESS) {
            window.setShouldClose(true);
        }

        // TODO: Move to camera control class
        if (!changeSpeedManually) {
            if (window.getKey(GLFW_KEY_LEFT_SHIFT) == Input::InputType::PRESS) {
                moveSpeed = 0.12f;
            } else {
                moveSpeed = 0.04f;
            }
        }
        if (window.getKey(GLFW_KEY_W) == Input::InputType::PRESS) {
            camera.moveForward(moveSpeed);
        }
        if (window.getKey(GLFW_KEY_S) == Input::InputType::PRESS) {
            camera.moveForward(-moveSpeed);
        }
        if (window.getKey(GLFW_KEY_A) == Input::InputType::PRESS) {
            camera.moveRight(-moveSpeed);
        }
        if (window.getKey(GLFW_KEY_D) == Input::InputType::PRESS) {
            camera.moveRight(moveSpeed);
        }
        if (window.getKey(GLFW_KEY_J) == Input::InputType::PRESS) {
            scene.setActiveCamera(&camera1);
        }
        if (window.getKey(GLFW_KEY_K) == Input::InputType::PRESS) {
            scene.setActiveCamera(&camera);
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

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Debug Panel");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        if (ImGui::Button(changeSpeedManually ? "Change Speed Manually: ON" : "Change Speed Manually: OFF")) {
            changeSpeedManually = !changeSpeedManually;
        }
        if (ImGui::Button(lockFPS ? "Lock FPS" : "Unlock FPS")) {
            if (lockFPS) {
                glfwSwapInterval(1);
                lockFPS = false;
            } else {
                glfwSwapInterval(0);
                lockFPS = true;
            }
        }
        ImGui::SliderFloat("Move Speed", &moveSpeed, 0.01f, 0.5f);
        ImGui::End();

        ImGui::Begin("Objects");
        ImGui::Text("Objects");
        if (ImGui::Button(showAddObjects ? "Close Object adder menu" : "Open Object adder menu")) {
            showAddObjects = !showAddObjects;
        }

        while (objectEulerAngles.size() < scene.getGameObjects().size()) {
            int idx = objectEulerAngles.size();
            glm::vec3 euler = glm::degrees(glm::eulerAngles(scene.getGameObjects()[idx].getTransform().getRotation()));
            objectEulerAngles.push_back(euler);
        }
        if (objectEulerAngles.size() > scene.getGameObjects().size()) {
            objectEulerAngles.resize(scene.getGameObjects().size());
        }

        for (int i = 0; i < scene.getGameObjects().size();) {
            ImGui::Text("GameObject %d", i);
            ImGui::PushID(i);
            if (ImGui::Button("Remove GameObject")) {
                scene.getGameObjects().erase(scene.getGameObjects().begin() + i);
                objectEulerAngles.erase(objectEulerAngles.begin() + i);
                ImGui::PopID();
                continue;
            }

            ImGui::Text("Position for object %d", i);
            float xPos = scene.getGameObjects()[i].getTransform().getPosition().x;
            float yPos = scene.getGameObjects()[i].getTransform().getPosition().y;
            float zPos = scene.getGameObjects()[i].getTransform().getPosition().z;
            ImGui::SliderFloat("X Position", &xPos, -10.0f, 10.0f);
            ImGui::SliderFloat("Y Position", &yPos, -10.0f, 10.0f);
            ImGui::SliderFloat("Z Position", &zPos, -10.0f, 10.0f);
            scene.getGameObjects()[i].getTransform().setPosition({xPos, yPos, zPos});

            ImGui::Text("Rotation for object %d", i);
            ImGui::SliderFloat("X Rotation", &objectEulerAngles[i].x, -180.0f, 180.0f);
            ImGui::SliderFloat("Y Rotation", &objectEulerAngles[i].y, -180.0f, 180.0f);
            ImGui::SliderFloat("Z Rotation", &objectEulerAngles[i].z, -180.0f, 180.0f);
            glm::quat newRotation = glm::quat(glm::radians(objectEulerAngles[i]));
            scene.getGameObjects()[i].getTransform().setRotation(newRotation);

            ImGui::Text("Scale for object %d", i);
            float xScale = scene.getGameObjects()[i].getTransform().getScale().x;
            float yScale = scene.getGameObjects()[i].getTransform().getScale().y;
            float zScale = scene.getGameObjects()[i].getTransform().getScale().z;
            ImGui::SliderFloat("X Scale", &xScale, -10.0f, 10.0f);
            ImGui::SliderFloat("Y Scale", &yScale, -10.0f, 10.0f);
            ImGui::SliderFloat("Z Scale", &zScale, -10.0f, 10.0f);
            scene.getGameObjects()[i].getTransform().setScale({xScale, yScale, zScale});

            ImGui::PopID();
            i++;
        }
        ImGui::End();

        if (showAddObjects) {
            ImGui::Begin("Object Adder");
            ImGui::Text("Mesh settings");
            ImGui::InputText("Path to .obj", objPath, 256);

            ImGui::Text("Material Settings");
            ImGui::ColorPicker4("Color", glm::value_ptr(color));
            ImGui::InputText("Path to texture", texturePath, 256);

            ImGui::Text("Spawn settings");
            ImGui::Text("   Position");
            ImGui::SliderFloat("X Position", &tempPosition.x, -10.0f, 10.0f);
            ImGui::SliderFloat("Y Position", &tempPosition.y, -10.0f, 10.0f);
            ImGui::SliderFloat("Z Position", &tempPosition.z, -10.0f, 10.0f);
            ImGui::Text("   Rotation");
            ImGui::SliderFloat("X Rotation", &tempRotationEuler.x, -180.0f, 180.0f);
            ImGui::SliderFloat("Y Rotation", &tempRotationEuler.y, -180.0f, 180.0f);
            ImGui::SliderFloat("Z Rotation", &tempRotationEuler.z, -180.0f, 180.0f);
            ImGui::Text("   Scale");
            ImGui::SliderFloat("X Scale", &tempScale.x, 0.01f, 10.0f);
            ImGui::SliderFloat("Y Scale", &tempScale.y, 0.01f, 10.0f);
            ImGui::SliderFloat("Z Scale", &tempScale.z, 0.01f, 10.0f);

            tempGameObject.getTransform().setPosition(tempPosition);
            tempGameObject.getTransform().setRotation(glm::quat(glm::radians(tempRotationEuler)));
            tempGameObject.getTransform().setScale(tempScale);

            if (ImGui::Button("Add GameObject to scene")) {
                std::string contents = loadShaderFromPath(std::string(objPath).c_str());
                parser.source(&contents);
                Mesh::Mesh* newMesh = new Mesh::Mesh(parser.parse());
                newMesh->build();

                Texture::Texture* newTexture = new Texture::Texture();
                if (!newTexture->load(std::string(texturePath))) {
                    std::cerr << "Failed to load texture: " << texturePath << std::endl;
                }

                Material::Material* newMaterial = new Material::Material();
                newMaterial->setTexture(newTexture);
                newMaterial->setColor(color);
                newMaterial->setShader(&shaderProgram);

                Rendering::MeshRenderer* newMR = new Rendering::MeshRenderer();
                newMR->setMesh(*newMesh);
                newMR->setMaterial(*newMaterial);

                GameObject::GameObject* newGO = new GameObject::GameObject();
                newGO->setMeshRenderer(*newMR);
                newGO->setTransform(Transform::Transform(
                    tempPosition,
                    glm::quat(glm::radians(tempRotationEuler)),
                    tempScale
                ));

                scene.addGameObject(newGO);
                objectEulerAngles.push_back(tempRotationEuler);

                tempPosition = glm::vec3(0.0f);
                tempRotationEuler = glm::vec3(0.0f);
                tempScale = glm::vec3(1.0f);
                memset(objPath, 0, sizeof(objPath));
                memset(texturePath, 0, sizeof(texturePath));
            }
            ImGui::End();
        }

        renderer.render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.swapBuffers();
        window.pollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    window.shutdown();
    return 0;
}
