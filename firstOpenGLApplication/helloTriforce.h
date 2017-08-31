#ifndef HELLO_TRIFORCE
#define HELLO_TRIFORCE

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "dynamicArray.c"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);						// handles window resizing
void processInput(GLFWwindow *window, int * fPtr);												// processes when keys are pressed/released and responds
void initWindow();																				// initializes GLFW Window
GLFWwindow* makeWindow(int width, int height, char* name);										// create window object
void initGLAD();																				// initialize GLAD to manage function pointers for OpenGL
unsigned int * makeShader(const char** ShaderSrc, int i, DynArr* trash);						// creates a shader given a shader source
unsigned int * makeShaderProgram(unsigned int* vShader, unsigned int* fShader, DynArr* trash);
unsigned int * makeVAO(DynArr* trash);
void render(GLFWwindow* win, unsigned int* shaderProg[], unsigned int* VAO);
void emptyTrashCan(DynArr * tc);

#endif
