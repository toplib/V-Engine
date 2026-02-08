#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "debug/Logger.h"
#include "test.h"
#include "platform/shader/Shader.h"
#include "platform/shader/ShaderType.h"
#include "platform/shader/ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static debug::Logger logger("main");

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    // GLFW Init
    logger.info() << "Setting up GLFW window hints";
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Window creation
    logger.info() << "Creating window";
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "V-Engine", NULL, NULL);
    if (window == NULL) {
        logger.error() << "Failed to create GLFW window";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD
    logger.info() << "Initializing GLAD";
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        logger.error() << "Failed to initialize GLAD";
        return -1;
    }

    // Viewport
    logger.info() << "Setting viewport";
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Vertex data
    float vertices[] = {
        // positions         // colors          // texture coords
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f    // top left
    };

    unsigned int indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };

    // Generate VAO, VBO, EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Load texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // Flip texture vertically
    unsigned char* data = stbi_load("/home/toplib/V-Engine/res/texture.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        logger.error() << "Failed to load texture";
    }
    logger.print() << data;
    stbi_image_free(data);

    // Shaders
    Shader::Shader vertexShader(Shader::ShaderType::VERTEX);
    Shader::Shader fragmentShader(Shader::ShaderType::FRAGMENT);

    vertexShader.source(loadVertexShader());
    fragmentShader.source(loadFragmentShader());

    try {
        if (!vertexShader.compile() || !fragmentShader.compile()) {
            logger.error() << "Cannot compile shaders";
            return -1;
        }
    } catch (char* str) {
        logger.error() << "Shader compilation exception: " << str;
    }

    Shader::ShaderProgram shaderProgram;
    shaderProgram.attach(vertexShader);
    shaderProgram.attach(fragmentShader);

    try {
        if (!shaderProgram.link()) {
            logger.error() << "Cannot link shader program";
            return -1;
        }
    } catch (char* str) {
        logger.error() << "Shader linking exception: " << str;
    }

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);
        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
