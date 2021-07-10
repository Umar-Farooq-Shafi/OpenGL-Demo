#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

/*! @brief Resize the window.
 *
 *  This function resize the windows if user the
 *	try to change the window size.
 *
 *  @param[in] window pointer.
 *  @param[in] heigt
 *  @param[in] width
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/*! @brief Handling the user input.
 *
 *  This function handle input like key press to
 *  and if escape key is pressed than set
 *	glfwSetWindowShouldClosed to true to close the window
 *
 *  @param[in] window pointer.
 */
void processInput(GLFWwindow* window)
{
	// Check if esc key is pressed then close the window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	// Initialize the GLFW
	glfwInit();

	/*! @brief Config the GLFW
	*	Telling GLFW what version of openGL using
	*
	*	@param[in] GLFW_CONTEXT_VERSION_MAJOR { Hint }
	*   @param[in] 3.3 version of openGL
	*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Modern function from core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// For Mac OS

	/*! @brief Creating window or console by 800x600.
	*
	*	@param[in] WIDTH.
	*	@param[in] HEIGHT.
	*	@param[in] Name of window.
	*	@param[in] Monitor mode Full or not.
	*	@param[in] Share mode.
	*/
	GLFWwindow* window = glfwCreateWindow(
		800,
		600,
		"LearnOpenGL",
		NULL,
		NULL
	);

	// Checking if window is created successfully or not
	if (!window)
	{
		std::cerr << "[ERROR]Failed to creare GLFW window\n";
		glfwTerminate();
		return -1;
	}

	// Telling the glfw the window in the current context
	glfwMakeContextCurrent(window);

	/*! @brief Calling GLAD manager function to initialize.
	*
	*	This function bind GLAD before calling the OpenGL function.
	*
	*	@param[in] Address of OpenGL.
	*/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "[ERROR]Failed to initialized GLAD\n";
		return -1;
	}

	/*! @brief Setting viewport of window.
	*
	*	This function use openGL to create viewport of 800x600
	*
	*	@param[in] x co-ordinate.
	*	@param[in] y co-ordinate.
	*	@param[in] Width.
	*	@param[in] Height.
	*/
	glViewport(0, 0, 800, 600);

	//  tell GLFW we want to call this function on every window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Setting the vertices of triangle
	//			 y-axis
	//			(0, 0.5)
	// 
	// 
	//	(-0.5, -0.5)	(0.5, -0.5) x-axis
	// 
	float vertices[] = {
		// Positions			// Colors
		 .5f, -.5f, .0f,		1.0f,  .0f,  .0f,  // bottom right
		-.5f, -.5f, .0f, 		 .0f, 1.0f,  .0f,  // bottom left
		 .0f,  .5f, .0f,		 .0f,  .0f, 1.0f   // top middle
	};

	/*! @brief Generating buffer.
	*
	*	Buffer is simply memory in GPU.
	*
	*	@param[in] Numbers of buffer.
	*	@param[out] Assign generated buffer to VBO
	*/
	unsigned int VBO;
	glGenBuffers(1, (GLuint*)&VBO);

	// vetex buffer array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);

	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(
		GL_ARRAY_BUFFER, // GL_ARRAY_BUFFER is type of vertex buffer
		VBO				 // Buffer to bind
	);

	/*
		glBufferData function that copies the previously defined
		vertex data into the buffer’s memory
		@param
		type of buffer we want to copy
		size of data
		actual data
		behavior to manage the given data
		@endparam
	*/
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(vertices),
		vertices,
		GL_STATIC_DRAW		// data is set once and used many times
	);

	Shader shader("Shader.vs", "Shader.fs");

	// TODO: draw a rectangle
	// drawingRectangle();

	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(
		0,					// position { layout (location = 0) }
		3,					// number of vertex e.g vec3 so 3
		GL_FLOAT,			// type of vertex co-ordinates
		GL_FALSE,			// is it int, byte
		6 * sizeof(float),	// next shader start
		(void*)0			// offset
	);
	// Enabling the vertex array
	glEnableVertexAttribArray(0);

	// 4. color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Enabling wireframe mode
	glPolygonMode(
		GL_FRONT_AND_BACK,
		GL_LINE // If you want frame mode set this to GL_LINE.
	);

	// Forcing winow to open
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		// clear the colorbuffer
		// Specifying the background color of window in rgba
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Clean the background color and setting new color
		glClear(GL_COLOR_BUFFER_BIT);

		// Activate the shader
		shader.use();

		// 4. draw the object
		glBindVertexArray(VAO);

		// Drawing the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap the backg buffer with front buffer
		glfwSwapBuffers(window);

		// rendering the window 
		// Taking care of all events
		// Check and call events and swap the buffer
		glfwPollEvents();
	}

	// Killing the default window
	glfwDestroyWindow(window);
	// Deleting all windows resorces 
	glfwTerminate();
	return 0;
}
