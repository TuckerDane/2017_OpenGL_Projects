//************************************************************************************************************************
//
//	LearnOpenGL - helloTriforce.h
//
//	Name:			Tucker Dane Walker
//	Date:			August 2017
//	Description:	Specifications for rendering a Triforce and other shapes using OpenGL.
//
//***********************************************************************************************************************/

#ifndef HELLO_TRIFORCE
#define HELLO_TRIFORCE

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "dynamicArray.c"

// GLAD
//---------------------------------
void initGLAD();																				// initialize GLAD to manage function pointers for OpenGL

// WINDOW
//---------------------------------
void initWindow();																				// initializes GLFW Window
GLFWwindow* makeWindow(int width, int height, char* name);										// create window object
void framebuffer_size_callback(GLFWwindow* window, int width, int height);						// handles window resizing

// SHADERS / VAOs
//---------------------------------
unsigned int * makeShader(const char** shaderSrc, int i, DynArr* trash);						// creates a shader given a shader source
unsigned int * makeShaderProgram(unsigned int* vShader, unsigned int* fShader, DynArr* trash);	// creates a shader program
unsigned int ** makeVAOs(DynArr* trash, int numVAOs);											// creates VAOs

// RENDERING
//---------------------------------
void processInput(GLFWwindow *window, int * fPtr, int *tPtr);									// processes when keys are pressed/released and responds
void render(GLFWwindow* win, unsigned int* shaderProg[], unsigned int* VAO, int numVAOs);		// render loop

//	GARBAGE COLLECTION
//---------------------------------
void emptyTrashCan(DynArr * tc);

#endif
