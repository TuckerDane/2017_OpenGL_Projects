#include "helloTriforce.h"
#include <assert.h>

// tool for debugging
#define _CRTDBG_MAP_ALLOC
#ifndef _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif


//	settings
const unsigned int	SCR_WIDTH = 800;
const unsigned int	SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char *fragmentShader1Source = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
	"}\n\0";

const char *fragmentShader2Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";

const char *fragmentShader3Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n\0";

int main()
{
	// trashcan used for dynamic memory cleanup
	DynArr * trashcan = NULL;
	trashcan = new DynArr;
	assert(trashcan != 0);
	initDynArr(trashcan, 10);
	
	// make window
	initWindow();															// initialize GLFW Window
	GLFWwindow *window = makeWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOPenGL"); 	// create a window object
	initGLAD();																// initialize GLAD to manage function pointers for OpenGL

	// make shaders
	unsigned int * vertexShader = makeShader(&vertexShaderSource, 0, trashcan);				// vertex shader
	unsigned int * fragmentShaders[3];
	fragmentShaders[0] = makeShader(&fragmentShader1Source, 1, trashcan);	// fragment shader
	fragmentShaders[1] = makeShader(&fragmentShader2Source, 1, trashcan);
	fragmentShaders[2] = makeShader(&fragmentShader3Source, 1, trashcan);

	// make shader program
	unsigned int * shaderPrograms[3];
	for (int i = 0; i < 3; i++)
	{
		shaderPrograms[i] = makeShaderProgram(vertexShader, fragmentShaders[i], trashcan);			// shader linking
	}

	// make VAO
	unsigned int ** VAOs = makeVAOs(trashcan, 3);

	// render loop
	render(window, shaderPrograms, *VAOs, 3);

	// garbage collection
	emptyTrashCan(trashcan);
	glfwTerminate();
	
	// tool for debugging
	_CrtDumpMemoryLeaks();

	return 0;
}

//-------------------------------------------------------------------
// Initialize GLFW Window
//-------------------------------------------------------------------
void initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

//-------------------------------------------------------------------
//	handles window resizing
//-------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//-------------------------------------------------------------------
//	processes when keys are pressed/released and responds
//-------------------------------------------------------------------
void processInput(GLFWwindow *window, int * fPtr)
{
	// handle exiting
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	// handle different fragShaders
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		*fPtr = 0;
	}
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		*fPtr = 1;
	}
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		*fPtr = 2;
	}

	// handle polygon mode vs solid fill
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

//-------------------------------------------------------------------
// Create a window object
//-------------------------------------------------------------------
GLFWwindow* makeWindow(int width, int height, char* name)
{
	GLFWwindow* win = glfwCreateWindow(width, height, name, NULL, NULL);
	if (win == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		assert(0);
	}
	glfwMakeContextCurrent(win);
	glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);	//handles window resizing

	return win;
}

//-------------------------------------------------------------------
// initialize GLAD to manage function pointers for OpenGL
//-------------------------------------------------------------------
void initGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		assert(0);
	}
}

//-------------------------------------------------------------------
// creates a shader given a shader source
//-------------------------------------------------------------------
unsigned int * makeShader(const char** ShaderSrc, int i, DynArr* trash)
{
	//	initialize shader
	unsigned int * shadr = NULL;
	shadr = new unsigned int;
	addDynArr(trash, shadr);

	if (i == 0)	// vertex shader
	{
 		*shadr = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (i == 1)	// fragment shader
	{
		*shadr = glCreateShader(GL_FRAGMENT_SHADER);
	}

	// attach shader source code to the shader object and compile
	glShaderSource(*shadr, 1, ShaderSrc, NULL);
	glCompileShader(*shadr);

	// check for compilation errors:
	int success;
	char infoLog[512];
	glGetShaderiv(*shadr, GL_COMPILE_STATUS, &success);

	// print error message if compilation errors exist
	if (!success)
	{
		glGetShaderInfoLog(*shadr, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return shadr;
}

//-------------------------------------------------------------------
// shader program
//-------------------------------------------------------------------
unsigned int * makeShaderProgram(unsigned int* vShader, unsigned int* fShader, DynArr* trash)
{
	int success;
	char infoLog[512];

	// initialize shader program
	unsigned int * sProg = NULL;
	sProg = new unsigned int;
	addDynArr(trash, sProg);
	*sProg = glCreateProgram();

	// link precompiled shaders to shader program
	glAttachShader(*sProg, *vShader);
	glAttachShader(*sProg, *fShader);
	glLinkProgram(*sProg);

	// check for linker errors
	glGetProgramiv(*sProg, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(*sProg, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// garbage collection on shader objects
	glDeleteShader(*vShader);
	glDeleteShader(*fShader);

	return sProg;
}

//-------------------------------------------------------------------
// render loop
//-------------------------------------------------------------------
void render(GLFWwindow* win, unsigned int* shaderProg[], unsigned int* VAO, int numVAOs)
{
	int frag = 0;
	int * fragPtr = &frag;
	int vArrayObj = 0;
	int * vArrayObjPtr = &vArrayObj;

	while (!glfwWindowShouldClose(win))
	{
		// input
		processInput(win, fragPtr);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw vertices
		glUseProgram(*shaderProg[frag]);

		for (int i = 0; i < numVAOs; i++)
		{
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);
		}
		
		// check and call events and swap the buffers
		glfwSwapBuffers(win);
		glfwPollEvents();
	}
}

//-------------------------------------------------------------------
// vertex data :: buffer(s) :: vertex attributes
//-------------------------------------------------------------------
unsigned int ** makeVAOs(DynArr* trash, int numVAOs)
{
	// vertex data
	float triangle1[] = {
		 // First Triangle
		 0.00f,	 0.50f,	 0.00f,	// 0
		-0.25f,	 0.00f,	 0.00f,	// 1
		 0.25f,	 0.00f,	 0.00f,	// 2
	};

	float triangle2[] = {
		// Second Triangle
		-0.25f,	 0.00f,	 0.00f,	// 1
		-0.50f,	-0.50f,	 0.00f,	// 3
		0.00f,	-0.50f,	 0.00f,	// 4
	};

	float triangle3[] = {
		// Third Triangle
		0.25f,	 0.00f,	 0.00f,	// 2
		0.00f,	-0.50f,	 0.00f,	// 4
		0.50f,	-0.50f,	 0.00f	// 5
	};

	float * triangles[]
	{
		triangle1,
		triangle2,
		triangle3
	};

	unsigned int * VAOs = NULL;
	VAOs = new unsigned int[3];

	addDynArr(trash, VAOs);

	unsigned int VBOs[3];// , VAOs[3]; // , EBO;

	// generate vertex arrays and buffers
	glGenVertexArrays(numVAOs, VAOs);
	glGenBuffers(numVAOs, VBOs);
	//glGenBuffers(1, &EBO);

	for (int i = 0; i < numVAOs; i++)
	{
		// bind vertex array object
		glBindVertexArray(VAOs[i]);

		// copy vertices into a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangles)*3, triangles[i], GL_STATIC_DRAW);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// set the vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	return &VAOs;
}

//-------------------------------------------------------------------
// garbage collection function
//-------------------------------------------------------------------
void emptyTrashCan(DynArr * tc)
{
	for (int i = 0; i < tc->size; i++)
	{
		delete tc->data[i];
	}
	deleteDynArr(tc);
}