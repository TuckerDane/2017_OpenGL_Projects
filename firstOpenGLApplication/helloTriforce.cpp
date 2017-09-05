//************************************************************************************************************************
//
//	LearnOpenGL - helloTriforce.cpp
//
//	Name:			Tucker Dane Walker
//	Date:			August 2017
//	Description:	Implementation for rendering a Triforce and other shapes using OpenGL.
//
//***********************************************************************************************************************/

#define _CRTDBG_MAP_ALLOC
#include<iostream>
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include "helloTriforce.h"
#include "shader.h"
#include <assert.h>


//-------------------------------------------------------------------
//	settings
//-------------------------------------------------------------------

// size of the window to render triforce/shapes to
const unsigned int	SCR_WIDTH = 800;
const unsigned int	SCR_HEIGHT = 600;

int main()
{
	// tool for debugging
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// trashcan used for dynamic memory cleanup
	//---------------------------------
	DynArr * trashcan = NULL;
	trashcan = new DynArr;
	assert(trashcan != 0);
	initDynArr(trashcan, 10);

	// make window
	//---------------------------------
	initWindow();															// initialize GLFW Window
	GLFWwindow *window = makeWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOPenGL"); 	// create a window object
	initGLAD();																// initialize GLAD to manage function pointers for OpenGL

	// make shader programs
	//---------------------------------
	
	std::string filePath;			// holds filepath for fragment shader
	Shader shaders[5];				// holds all of the shader programs
	unsigned int * sProgIDs[5];		// holds the shader program IDs

	for (int i = 0; i < 5; i++)
	{
		// put the filepath into a string
		filePath = "shaders/fragmentShader";
		filePath += (i+48);
		filePath += ".fs.txt";
		const char* fPath = filePath.c_str();
		Shader sProg("shaders/vertexShader1.vs.txt", fPath);
		shaders[i] = sProg;
		sProgIDs[i] = &shaders[i].ID;
		filePath.clear();
	}	

	// make VAO
	//---------------------------------
	unsigned int** VAOs = makeVAOs(trashcan, 3);

	// render loop
	//---------------------------------
	render(window, sProgIDs, *VAOs, 3);

	// garbage collection
	//---------------------------------
	emptyTrashCan(trashcan);
	glfwTerminate();

	return 0;
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
// Create a window object
//
//	@param:		width		width of the window in pixels
//	@param:		height		height of the window in pixels
//	@param:		name		name of the window as a string of chars
//	@return:	win			returns a window of size (width)*(height)
//							pixels named (name)
//-------------------------------------------------------------------
GLFWwindow* makeWindow(int width, int height, char* name)
{
	// create the window
	GLFWwindow* win = glfwCreateWindow(width, height, name, NULL, NULL);

	// ensure that the window was created
	if (win == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		assert(0);
	}
	glfwMakeContextCurrent(win); // ensures that win is the current context for glfw
	glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);	// handles window resizing

	return win;
}

//-------------------------------------------------------------------
//	Handles window resizing
//
//	@param:	window			window that is rendered to
//	@param:	window width	width of the window in pixels
//	@param:	window height	height of the window in pixels
//-------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//-------------------------------------------------------------------
// vertex data :: buffer(s) :: vertex attributes
//
//	@param:		trash		the global trashcan for garbage collection
//	@param:		numVAOs		the number of VAOs being passed
//	@return:	VAOs		returns the address to the VAOs array
//							which contains reference IDs to VAOs
//-------------------------------------------------------------------
unsigned int ** makeVAOs(DynArr* trash, int numVAOs)
{
	// vertex data
	//---------------------------------

	// first triangle (Top)
	float triangle1[] = {
		// positions			// colors
		 0.0f,	0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
		-0.25f,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		 0.25f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f
	};

	//	second triangle (Left)
	float triangle2[] = {
		// positions			// colors
		-0.25f,	 0.00f,	0.00f,	1.0f,	0.0f, 0.0f,
		-0.50f,	-0.50f,	0.00f,	0.0f,	1.0f, 0.0f,
		 0.00f, -0.50f,	0.00f,	0.0f,	0.0f, 1.0f
	};

	//	third triangle (Right)
	float triangle3[] = {
		// positions			// colors
		0.25f,	 0.00f,	0.00f,	1.0f,	0.0f, 0.0f,
		0.00f,	-0.50f,	0.00f,	0.0f,	1.0f, 0.0f,
		0.50f,  -0.50f,	0.00f,	0.0f,	0.0f, 1.0f
	};

	//	all triangles
	float * triangles[] = {
		triangle1,
		triangle2,
		triangle3
	};

	// buffers
	//---------------------------------
	unsigned int * VAOs = NULL;
	VAOs = new unsigned int[3];

	addDynArr(trash, VAOs);

	unsigned int VBOs[3];

	//	generate vertex arrays and buffers
	//---------------------------------
	glGenVertexArrays(numVAOs, VAOs);
	glGenBuffers(numVAOs, VBOs);

	//	set attribute pointers of VAOs
	//---------------------------------
	for (int i = 0; i < numVAOs; i++)
	{
		// bind vertex array object
		glBindVertexArray(VAOs[i]);

		// copy vertices into a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangles) * 6, triangles[i], GL_STATIC_DRAW);

		// set the vertex attributes pointers

		// position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

		// color attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	}

	// return address to the VAOs array
	return &VAOs;
}

//-------------------------------------------------------------------
//	processes when keys are pressed/released and responds
//
//	@param: window		window that is rendered to
//	@param: fPtr		pointer to an integer that switches which
//						frag shader is used inside of the render loop
//							0 == Blue
//							1 == Yellow
//							2 == Red
//							3 == White
//	@param:	tPtr		pointer to an integer that switches which 
//						triangle is being transformed inside of the
//						render loop
//							W == Top triangle (triangle 1)
//							A == Left Triangle (triangle 2)
//							D == Right Triangle (triangle 3)
//	@param:	bPtr		pointer to a int which determines if
//						blink is on/off
//							0 == off
//							1 == on
//-------------------------------------------------------------------
void processInput(GLFWwindow *window, int * fPtr, int * tPtr, int * bPtr)
{
	// if the user presses ESCAPE, close the window and exit rendering
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	// change between shaders
	//---------------------------------

	// if the user presses '1', switch to the Blue Shader
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		*fPtr = 0;
	}
	// else if the user presses '2', switch to the Yellow Shader
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		*fPtr = 1;
	}
	// else if the user presses '3', switch to the Red Shader
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		*fPtr = 2;
	}
	
	//else if the user presses '4', switch to the Interpolated Shader
	else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		*fPtr = 3;
	}
	/*													
	// else if the user presses '5', switch to the White Shader
	else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	{
		*fPtr = 4;
	}														*/

	// change between triangles
	//---------------------------------

	// if the user presses 'W', switch to the Top Triangle
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		*tPtr = 0;
	}
	// else if the user presses 'A', switch to the Left Triangle
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		*tPtr = 1;
	}
	// else if the user presses 'D', switch to the Right Triangle
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		*tPtr = 2;
	}

	// change between polygon and fill
	//---------------------------------

	// if the user presses 'P', switch to Polygon (wireframe) Mode
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	// else if the user presses 'F', switch to Fill (solid) mode
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// turn blink on/off
	//---------------------------------

	// if the user presses 'B', switch to increase Saturation
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	{
		*bPtr = 1;
	}
	// else if user presses "V", switch to decrease Saturation
	else if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
	{
		*bPtr = 0;
	}
}

//-------------------------------------------------------------------
// render loop - renders pixels to a window
//
//	@param:		win			the window to be rendered to
//	@param:		shaderProg	an array of shader program reference IDs
//	@param:		VAO			reference ID to a Virtual Array Object
//	@param:		numVAOs		the number of VAOs being passed
//-------------------------------------------------------------------
void render(GLFWwindow* win, unsigned int * shaderProg[], unsigned int* VAO, int numVAOs)
{
	// determines which fragmentation shader is in current use
	//---------------------------------
	int currentFrag = 0;
	int * currentFragPtr = &currentFrag;
	int prevFrag = 0;

	// determines which triangle is selected for manipulation
	//---------------------------------
	int currentTriangle = 0;
	int * currentTriPtr = &currentTriangle;
	int prevTriangle = 0;

	// turns blinking off and on
	//---------------------------------
	int blink = 0;
	int * blinkPtr = &blink;

	float satValue = 1.0f;

	// holds which triangle is which color
	//---------------------------------
	int triangleColors[3] = {
		0,	1,	2				// colors	 (0 = Blue,	1 = Yellow, 2 = Red)
	};

	//---------------------------------
	// render loop
	//---------------------------------
	while (!glfwWindowShouldClose(win))
	{
		// process state changes via input
		//---------------------------------
		processInput(win, currentFragPtr, currentTriPtr, blinkPtr);

		// set the color of the current triangle selected
		//---------------------------------
		// Same Triangle
		if (currentTriangle == prevTriangle)
		{
			// Different Color
			if (currentFrag != prevFrag)
			{
				triangleColors[currentTriangle] = currentFrag;		// change the color of the current triangle
				prevFrag = currentFrag;								// and update the previously selected color
			}
		}
		// Different Triangle
		else // (currentTriangle != prevTriangle)
		{
			prevFrag = triangleColors[currentTriangle];	
			currentFrag = prevFrag;				
			prevTriangle = currentTriangle;		
		}

		// render
		//---------------------------------
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw to the window buffer
		//---------------------------------

		// Change the color of the selected triangle to White when selected

		// Top Triangle
		if ((glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS))
		{
			for (int i = 0; i < numVAOs; i++)
			{
				if (i == 0)
				{
					glUseProgram(*shaderProg[4]);					// use the white shader on the top triangle
					glBindVertexArray(VAO[i]);
					glDrawArrays(GL_TRIANGLES, 0, 3);
					glBindVertexArray(0);

				}
				else
				{
					glUseProgram(*shaderProg[triangleColors[i]]);	// and the correct shaders on the others
					glBindVertexArray(VAO[i]);
					glDrawArrays(GL_TRIANGLES, 0, 3);
					glBindVertexArray(0);
				}
			}
		}
		// If the Left Triangle is currently being selected
		else if ((glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS))
		{
			for (int i = 0; i < numVAOs; i++)
			{
				if (i == 1)
				{
					glUseProgram(*shaderProg[4]);					// use the white shader on the left triangle
					glBindVertexArray(VAO[i]);
					glDrawArrays(GL_TRIANGLES, 0, 3);
					glBindVertexArray(0);

				}
				else
				{
					glUseProgram(*shaderProg[triangleColors[i]]);	// and the correct shaders on the others
					glBindVertexArray(VAO[i]);
					glDrawArrays(GL_TRIANGLES, 0, 3);
					glBindVertexArray(0);
				}
			}
		}
		// Right Triangle
		else if ((glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS))
		{
			for (int i = 0; i < numVAOs; i++)
			{
				if (i == 2)
				{
					glUseProgram(*shaderProg[4]);					// use the white shader on the right triangle
					glBindVertexArray(VAO[i]);
					glDrawArrays(GL_TRIANGLES, 0, 3);
					glBindVertexArray(0);

				}
				else
				{
					glUseProgram(*shaderProg[triangleColors[i]]);	// and the correct shaders on the others
					glBindVertexArray(VAO[i]);
					glDrawArrays(GL_TRIANGLES, 0, 3);
					glBindVertexArray(0);
				}
			}
		}
		// No triangle currently being selected; Render normally
		else
		{
			// determine current color saturation
			//---------------------------------
			int vertexSatLocation = -1;												// initialize vertexSatLocation

			// change saturation
			if (blink == 1)
			{
				float timeValue = glfwGetTime();									// used to create saturation change
				satValue = (sin((1.5 * timeValue) + satValue) / 2.0) + 0.5f;		// rate of saturation change

				for (int i = 0; i < numVAOs; i++)
				{
					glUseProgram(*shaderProg[triangleColors[i]]);					// determine which shader program to draw with
					vertexSatLocation = glGetUniformLocation(*shaderProg[triangleColors[i]], "saturation");
					glUniform1f(vertexSatLocation, satValue);
					glBindVertexArray(VAO[i]);
					glDrawArrays(GL_TRIANGLES, 0, 3);
					glBindVertexArray(0);
				}
			}
			else // (blink != 1) - dont change saturation
			{
				for (int i = 0; i < numVAOs; i++)
				{
					glUseProgram(*shaderProg[triangleColors[i]]);					// determine which shader program to draw with
					vertexSatLocation = glGetUniformLocation(*shaderProg[triangleColors[i]], "saturation");
					glUniform1f(vertexSatLocation, satValue);
					glBindVertexArray(VAO[i]);
					glDrawArrays(GL_TRIANGLES, 0, 3);
					glBindVertexArray(0);
				}
			}
		}
		
		// check and call events and swap the buffers
		//---------------------------------
		glfwSwapBuffers(win);
		glfwPollEvents();
	}
}

//-------------------------------------------------------------------
// garbage collection function
//
//	@param:		tc		points to a trashcan filled with pointers
//						to dynamically allocated memory
//-------------------------------------------------------------------
void emptyTrashCan(DynArr * tc)
{
	// iterate through the trashcan and free its contents
	for (int i = 0; i < tc->size; i++)
	{
		delete tc->data[i];
	}
	// delete the trashcan itself
	deleteDynArr(tc);
}