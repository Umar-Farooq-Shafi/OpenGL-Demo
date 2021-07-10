#include <iostream>

// glfw, glad, opengl header
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Math header
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Image processing header
#include "stb_image.h"

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
void framebuffer_size_callback(GLFWwindow* window, GLsizei width, GLsizei height)
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
#ifdef __APPLE__	// For mac OS
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

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
	//  tell GLFW we want to call this function on every window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

	// build and compile our shader zprogram
	// ------------------------------------
	Shader shader("Shader.vs", "Shader.fs");

	/*! @brief Setting viewport of window.
	*
	*	This function use openGL to create viewport of 800x600
	*
	*	@param[in] x co-ordinate.
	*	@param[in] y co-ordinate.
	*	@param[in] Width.
	*	@param[in] Height.
	*/
	glViewport((GLint)0, (GLint)0, (GLsizei)800, (GLsizei)600);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	GLfloat vertices[] = {
		// Positions		// Colors			// texture co-ordinates 
		 .5f,  .5f, .0f,	1.0f,  .0f,  .0f,	1.0f, 1.0f, // top right
		 .5f, -.5f, .0f, 	 .0f, 1.0f,  .0f,   1.0f,  .0f, // bottom right
		-.5f, -.5f, .0f,	 .0f,  .0f, 1.0f,    .0f,  .0f, // bottom left
		-.5f,  .5f, .0f,	1.0f, 1.0f,  .0f,	 .0f, 1.0f  // top left
	};
	GLuint indices[] = {
	   0, 1, 3, // first triangle
	   1, 2, 3  // second triangle
	};

	/*! @brief Generating buffer.
	*
	*	Buffer is simply memory in GPU.
	*	Generating element buffer obj
	*	vetex buffer array object
	*
	*	@param[in] Numbers of buffer.
	*	@param[out] Assign generated buffer to VBO
	*/
	GLuint VBO, EBO, VAO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
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
		(GLsizeiptr)sizeof(vertices),
		(const void*)vertices,
		GL_STATIC_DRAW		// data is set once and used many times
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		(GLsizeiptr)sizeof(indices), (const void*)indices, GL_STATIC_DRAW);

	// 3. then set our position attributes pointers
	glVertexAttribPointer(
		(GLuint)0,					// position { layout (location = 0) }
		(GLint)3,					// number of vertex e.g vec3 so 3
		GL_FLOAT,			// type of vertex co-ordinates
		GL_FALSE,			// is it int, byte
		(GLsizei)8 * sizeof(float),	// next shader start
		(const void*)0			// offset
	);
	// Enabling the vertex array
	glEnableVertexAttribArray((GLuint)0);

	// 4. color attribute
	glVertexAttribPointer((GLuint)1, (GLint)3, GL_FLOAT,
		GL_FALSE, (GLsizei)8 * sizeof(float),
		(const void*)(3 * sizeof(float)));
	glEnableVertexAttribArray((GLuint)1);

	// 5. texture coord attribute
	glVertexAttribPointer((GLuint)2, (GLint)2, GL_FLOAT, 
		GL_FALSE, (GLsizei)8 * sizeof(float), 
		(const void*)(6 * sizeof(float)));
	glEnableVertexAttribArray((GLuint)2);

	// load and create a texture 
	// -------------------------
	GLuint texture1, texture2;
	glGenTextures(1, &texture1);
	// all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	glBindTexture(GL_TEXTURE_2D, texture1);

	// set the texture wrapping/filtering options (on currently bound texture)
	// Texture wrapping with texture co-ordinates
	glTexParameteri(
		GL_TEXTURE_2D,		// Target texture 2D, 3D or 1D
		GL_TEXTURE_WRAP_S,	// X co-ordinate s, t, r (x,y,z)
		GL_REPEAT	// repeat the mirror texture
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Mip map is used to render texture according to view how far texture is
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_LINEAR for smooth

	// load and generate the texture
	int width, height, nrChannel;
	unsigned char* data = stbi_load("container.jpg",
		&width, &height, &nrChannel, 0);
	if (!data)
	{
		std::cerr << "[ERROR]Failed to load texture\n";
		return -1;
	}
	glTexImage2D(
		GL_TEXTURE_2D,		// type of texture image 2D, 1D and so on
		(GLint)0,			// mipmap level
		GL_RGB,				// format of image to store
		(GLsizei)width,		// widht
		(GLsizei)height,	// height
		(GLint)0,			// this should always be 0
		GL_RGB, 			// format of source image
		GL_UNSIGNED_BYTE,	// datatype of source image
		(const void*)data	// actual data
	);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Free the image resources
	stbi_image_free(data);

	// Generating 2nd texture
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("./resources/textures/awesomeface.png",
		&width, &height, &nrChannel, 0);
	if (!data)
	{
		std::cerr << "[ERROR]Failed to load second texture resource\n";
		return -1;
	}
	glTexImage2D(GL_TEXTURE_2D, (GLint)0, GL_RGB, (GLsizei)width, 
		(GLsizei)height, (GLint)0, GL_RGBA, 
		GL_UNSIGNED_BYTE, (const void*)data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Free the image resources
	stbi_image_free(data);

	// scalling 50% down and rotating 90 degree angle in z-axis
	glm::mat4 trans = glm::translate(glm::mat4(1.f),
		glm::vec3(.5, -.5, .0f));
	// trans = glm::rotate(trans, glm::radians(90.f),
	//	 glm::vec3(.0f, .0f, 1.f));
	// trans = glm::scale(trans, glm::vec3(.5f, .5f, .5f));

	// create an orthographic projection matrix
	glm::ortho(.0f, 800.f, .0f, 600.f, .1f, 100.f);
	// perspective projection matrix
	glm::mat4 proj = glm::perspective(glm::radians(45.f),
		width / (float)height, .1f, 100.f);

	// manualy assign value
	shader.use();
	glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
	shader.setInt("texture2", 1); // or with shader class

	// Enabling wireframe mode
	glPolygonMode(
		GL_FRONT_AND_BACK,
		GL_FILL // If you want frame mode set this to GL_LINE.
	);

	// Forcing winow to open
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		// clear the colorbuffer
		// Specifying the background color of window in rgba
		glClearColor((GLfloat)0.2f, (GLfloat)0.3f, (GLfloat)0.3f, (GLfloat)1.0f);
		// Clean the background color and setting new color
		glClear(GL_COLOR_BUFFER_BIT);

		// bind Texture
		glActiveTexture(GL_TEXTURE0); // activate texture unit first
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// constantly rotating the container image
		trans = glm::rotate(trans, (float)glfwGetTime(),
			glm::vec3(.0f, .0f, 1.f));

		// Activate the shader
		shader.use();

		// rotate the container object
		glUniformMatrix4fv(
			glGetUniformLocation(shader.ID, (const GLchar*)"transform"),
			(GLsizei)1,								// # of matrix
			GL_FALSE,								// want to tanspose of matrix
			(const GLfloat*)glm::value_ptr(trans)	// actual matrix
		);

		// 4. draw the object
		glBindVertexArray(VAO);

		// Drawing the triangle
		glDrawElements(GL_TRIANGLES, (GLsizei)6, GL_UNSIGNED_INT, (const void*)0);

		// Swap the backg buffer with front buffer
		glfwSwapBuffers(window);

		// rendering the window 
		// Taking care of all events
		// Check and call events and swap the buffer
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteTextures(1, &texture1);
	glDeleteTextures(2, &texture2);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

