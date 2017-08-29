#include "helloRectangle.h"

//	settings
const unsigned int	SCR_WIDTH = 800;
const unsigned int	SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
	"}\n\0";

int main()
{
	//-------------------------------------------------------------------
	// Initialize GLFW Window
	//-------------------------------------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//-------------------------------------------------------------------
	// Create a window object
	//-------------------------------------------------------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOPenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	//handles window resizing

	//-------------------------------------------------------------------
	// initialize GLAD to manage function pointers for OpenGL
	//-------------------------------------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//-------------------------------------------------------------------
	// vertex shader
	//-------------------------------------------------------------------

	//	initialize vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// attach shader source code to the shader object and compile
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for compilation errors:
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	// print error message if compilation errors exist
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//-------------------------------------------------------------------
	// fragment shader
	//-------------------------------------------------------------------

	//	initialize fragmentShader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//	attach fragmentShader source code to the shader object and compile
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for compilation errors:
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	// print error message if compilation errors exist
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//-------------------------------------------------------------------
	// shader program
	//-------------------------------------------------------------------

	// initialize shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	// link precompiled shaders to shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linker errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// garbage collection on shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//-------------------------------------------------------------------
	// vertex data :: buffer(s) :: vertex attributes
	//-------------------------------------------------------------------

	// vertex data
	float vertices[] = {
		 0.5f,	 0.5f,	 0.0f,
		 0.5f,	-0.5f,	 0.0f,
		-0.5f,	-0.5f,	 0.0f,
		-0.5f,	 0.5f,	 0.0f
	};
	// index data
	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	// buffers
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// generate and bind vertex array object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	// copy vertices into a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// uncomment this call to draw in wireframe polygons
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//-------------------------------------------------------------------
	// render loop
	//-------------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//-------------------------------------------------------------------
	// garbage collection
	//-------------------------------------------------------------------
	glfwTerminate();
		
	return 0;
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
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}