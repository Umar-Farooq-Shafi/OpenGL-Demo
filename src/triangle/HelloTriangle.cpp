#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

/*! @brief Checking if compiling a shader is successfull or not.
*
*	This function get error log info and
*	print out to window
*
*	@param[in] shader object as refference.
*/
void checkingCopiledSuccess(unsigned int& shader, unsigned short&& status)
{
	int success;
	char infoLog[512];
	status == 0 ?
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success)
		:
		glGetShaderiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		abort();
	}
}

/*! @brief Drawing rectangle instead of triangle.
*
*	\n \n This function draw a rectangle in the window
*	using Element buffer object EBO instead of
*	Vertex Buffer object VBO. EBO eleminate duplicate
*	vertices and save memory like in VBO to draw a
*	rectangle: \n \n
*
*	float vertices[] = { \n
*		// first triangle \n
*		 0.5f,  0.5f, 0.0f, // top right \n
*		 0.5f, -0.5f, 0.0f, // bottom right \n
*		-0.5f,  0.5f, 0.0f, // top left \n
*		// second triangle \n
*		 0.5f, -0.5f, 0.0f, // bottom right \n
*		-0.5f, -0.5f, 0.0f, // bottom left \n
*		-0.5f,  0.5f, 0.0f  // top left \n
*	};
*/
void drawingRectangle()
{
	// Setting vertices of Trinagle
	float vertices[] = {
		 .5f,  .5f, .0f, // top right
		 .5f, -.5f, .0f, // bottom right
		-.5f, -.5f, .0f, // bottom left
		-.5f,  .5f, .0f  // top left
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// Generating element buffer object
	unsigned int EBO;
	glGenBuffers(1, (GLuint*)&EBO);

	// Binding the buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

// TODO: main()
int TODO()
{
	// Initialize the GLFW
	glfwInit();

	/*
		OpenGLSL shader language.
		vertex shader code.
		version 3.30.
		in is keyword specifies that it input var like aPos.
		gl_Position is var to store the generted shader.
	*/
	const char* vertexShaderSource =
		"#version 330 core\n\
		 layout (location = 0) in vec3 aPos;\n\
		 layout (location = 1) in vec3 aColor;\n\
		 \
		 out vec3 ourColor;\n\
		 \
		 void main()\n\
		 {\n\
			gl_Position = vec4(aPos, 1.f);\n\
			ourColor = aColor;\n\
		 }\0";
	// Vertex shader code using both in out and uniform
	/*
	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout ( location = 0 ) in vec3 aPos;\n"
		"out vec4 vertexColor;\n"
		"void main()\n{\n"
		" gl_Position = vec4(aPos, 1.0f);\n"
		" vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n}\0";
	*/

	/*
		fragment shader code.
		fragment is all about coloring every pixel in screen RGBA.
		out keyword specifies that it's output var.
		vec4 RGBA color value.
	*/
	const char* fragmentShaderSource =
		"#version 330 core\n\
		 out vec4 FragColor;\n\
		 in vec3 ourColor;\n\
		 \
		 void main()\n\
		 {\n\
			FragColor = ourColor;\n\
		 }\0";
	// Fragment shader code using uniform
	/*
	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n\n"
		"uniform vec4 ourColor;\n"
		"void main()\n{\n"
		" FragColor = ourColor;\n}\0";
	*/

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

	// Creating the shader type of vertex
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	/*
		attach the shader source code to the shader objectand compile the shader

		@param
		shader object to compile
		how many
		shader code
		@endparams
	*/
	glShaderSource(
		vertexShader,
		1,
		&vertexShaderSource,
		NULL
	);
	glCompileShader(vertexShader);

	// checking if shader is compile or failed
	checkingCopiledSuccess(vertexShader, 0);

	// Creating fragment shader and compiling same as vertex shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// checking if shader is compile or not
	checkingCopiledSuccess(fragmentShader, 0);

	// Attach all shader linking them into one program
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Checking if linking shader successfully
	checkingCopiledSuccess(shaderProgram, 1);

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

	// Method call to draw a rectangle
	// drawingRectangle();

	// 3. then set our vertex attributes pointers
	// Setting the var of vertex
	glVertexAttribPointer(
		0,					// position { layout (location = 0) }
		3,					// number of vertex e.g vec3 so 3
		GL_FLOAT,			// type of vertex co-ordinates
		GL_FALSE,			// is it int, byte
		3 * sizeof(float),	// next shader start
		(void*)0			// offset
	);
	// Enabling the vertex array
	glEnableVertexAttribArray(0);

	// Enabling wireframe mode
	glPolygonMode(
		GL_FRONT_AND_BACK,
		GL_FILL // If you want frame mode set this to GL_LINE.
	);

	// Getting maxim vertix attrib supported for this machine
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes
		<< std::endl;

	// Getting uniform var from shader code
	float timeValue, greenValue;
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

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
		glUseProgram(shaderProgram);

		// Using time to frequantly changing the color
		timeValue = glfwGetTime();
		greenValue = (sin(timeValue) / 2.f) + .5f;

		// 4. draw the object
		glUniform4f(vertexColorLocation, .0f, greenValue, .0f, 1.f);
		glBindVertexArray(VAO);

		// Drawing the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Drawing the rectangle
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0);

		// Swap the backg buffer with front buffer
		glfwSwapBuffers(window);

		// rendering the window 
		// Taking care of all events
		// Check and call events and swap the buffer
		glfwPollEvents();
	}

	// Killing the vertex and fragmnet shader
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	// Killing the default window
	glfwDestroyWindow(window);
	// Deleting all windows resorces 
	glfwTerminate();
	return 0;
}
