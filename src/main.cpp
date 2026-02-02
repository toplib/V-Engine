#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <debug/Logger.h>
#include "test.h"
#include "platform/shader/Shader.h"
#include "platform/shader/ShaderType.h"

static debug::Logger logger("main");
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main(){
  // GLFW Init
  logger.info() << "Setting up GLFW window hints";
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Window creation
  logger.info() << "Creating window";
  GLFWwindow* window = glfwCreateWindow(800, 600, "V-Engine", NULL, NULL);
  if (window == NULL){
    logger.error() << "Failed to create GLFW window";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // GLAD
  logger.info() << "Intializing GLAD";
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    logger.error() << "Failed to initializing GLAD";
    return -1;
  }

  // Viewport 
  logger.info() << "Setting viewport";
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);  
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  float vertices[] = {
    0.5f,  0.5f, 0.0f,   // top right
    0.5f, -0.5f, 0.0f,   // bottom right
   -0.5f, -0.5f, 0.0f,   // bottom left
   -0.5f,  0.5f, 0.0f    // top left
  };
  

  unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
  };



  logger.info() << "Setting up Array's";
  unsigned int VBO;
  glGenBuffers(1, &VBO);

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int EBO;
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Shaders
  Shader::Shader vertexShader(Shader::ShaderType::VERTEX);
  Shader::Shader fragmentShader(Shader::ShaderType::FRAGMENT);

  vertexShader.source(loadVertexShader());
  fragmentShader.source(loadFragmentShader());
  try {
      bool v_status = vertexShader.compile();
      bool f_status = fragmentShader.compile();

      if (!v_status || !f_status) {
        logger.error() << "Cannot compile shaders";
        return -1;
      }
  } catch (char* str){
    // Do nothing
  }

  // Shader program
  logger.info() << "Creating shader program";
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader.id());
  glAttachShader(shaderProgram, fragmentShader.id());
  logger.info() << "Linking program";
  glLinkProgram(shaderProgram);

  // Delete shaders because now we dont need it
  logger.info() << "Deleting shaders";
  //delete(vertexShader);
  //delete(fragmentShader);

  while(!glfwWindowShouldClose(window))
  {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    glfwSwapBuffers(window);
    glfwPollEvents();    
  }
  glfwTerminate();
  return 0;
}
