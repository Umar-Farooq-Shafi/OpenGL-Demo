#include "Shader.h"

Shader::Shader(
	const char* vertexPath,
	const char* fragmentPath,
	const char* geometryPath
)
{
	// 1. retrieve the vertex/fragment shader code from filePath
	std::string   vertexCode;
	std::string   fragmentCode;
	std::string   geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;

	// Ensure that source files can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// Open file
		vShaderFile  .open(vertexPath);
		fShaderFile  .open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		// Read's file buffer content into stream
		vShaderStream   << vShaderFile.rdbuf();
		fShaderStream   << fShaderFile.rdbuf();

		// close the file
		vShaderFile.close();
		fShaderFile.close();

		// Convert stream into string
		vertexCode   = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		if (geometryPath)
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure& f)
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	/*
		OpenGLSL shader language.
		vertex shader code.
		version 3.30.
		in is keyword specifies that it input var like aPos.
		gl_Position is var to store the generted shader.
	*/
	const char* vShaderCode   = vertexCode.c_str();
	/*
		fragment shader code.
		fragment is all about coloring every pixel in screen RGBA.
		out keyword specifies that it's output var.
		vec4 RGBA color value.
	*/
	const char* fShaderCode   = fragmentCode.c_str();

	// Creating the shader type of vertex
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER), 
		// Creating fragment shader and compiling same as vertex shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);

	/*
	*	vertex shader \n
		attach the shader source code to the shader objectand compile the shader

		@param
		shader object to compile
		how many
		shader code
		@endparams
	*/
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	// 2. compile the shaders
	glCompileShader(vertex);
	checkCompileError(vertex, "VERTEX");

	// fragment shader
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileError(fragment, "FRAGMENT");

	// geometry
	GLuint geometry;
	if (geometryPath)
	{
		const GLchar* gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompileError(geometry, "GEOMETRY");
	}

	// 3. shader program
	ID = glCreateProgram();
	// Attach all shader linking them into one program
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	if (geometryPath)
		glAttachShader(ID, geometry);
	glLinkProgram(ID);
	checkCompileError(ID, "PROGRAM");

	// Killing the vertex and fragmnet shader
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath)
		glDeleteShader(geometry);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setInt(const GLchar* name, GLint val) const
{
	glUniform1i(glGetUniformLocation(ID, name), val);
}

void Shader::setFloat(const GLchar* name, GLfloat val) const
{
	glUniform1f(glGetUniformLocation(ID, name), val);
}

void Shader::setVec2(const GLchar* name, const glm::vec2& value) const
{
	glUniform2fv(
		glGetUniformLocation(ID, name),
		1,
		&value[0]
	);
}

void Shader::setVec2(const GLchar* name, float x, float y) const
{
	glUniform2f(
		glGetUniformLocation(ID, name),
		x,
		y
	);
}

void Shader::setVec3(const GLchar* name, const glm::vec3& value) const
{
	glUniform3fv(
		glGetUniformLocation(ID, name),
		1,
		&value[0]
	);
}

void Shader::setVec3(const GLchar* name, float x, float y, float z) const
{
	glUniform3f(
		glGetUniformLocation(ID, name),
		x,
		y,
		z
	);
}

void Shader::setVec4(const GLchar* name, const glm::vec4& value) const
{
	glUniform4fv(
		glGetUniformLocation(ID, name),
		1,
		&value[0]
	);
}

void Shader::setVec4(const GLchar* name, 
	float x, float y, float z, float w) const
{
	glUniform4f(
		glGetUniformLocation(ID, name),
		(GLfloat)x,
		(GLfloat)y,
		(GLfloat)z,
		(GLfloat)w
	);
}

void Shader::setMat2(const GLchar* name, const glm::mat2& value) const
{
	glUniformMatrix2fv(
		glGetUniformLocation(ID, name),
		(GLsizei)1,
		GL_FALSE,
		(const GLfloat*)&value[0][0]
	);
}

void Shader::setMat3(const GLchar* name, const glm::mat3& value )const
{
	glUniformMatrix3fv(
		glGetUniformLocation(ID, name),
		(GLsizei)1,
		GL_FALSE,
		(const GLfloat*)&value[0][0]
	);
}

void Shader::setMat4(const GLchar* name, const glm::mat4& value) const
{
	glUniformMatrix4fv(
		glGetUniformLocation(ID, name),
		(GLsizei)1,
		GL_FALSE,
		(const GLfloat*)&value[0][0]
	);
}

void Shader::checkCompileError(GLuint& shader, std::string&& type) const
{
	GLint succes;
	GLchar infoLog[1024];
	
	if (type != "PROGRAM")
	{
		// print compile error if any
		glGetShaderiv(shader, GL_COMPILE_STATUS, &succes);
		if (!succes)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " 
				<< type << "\n" << infoLog 
				<< "\n -- --------------------------------------------------- -- " 
				<< std::endl;
		}
		else 
		{
			// print error if any
			glGetProgramiv(shader, GL_LINK_STATUS, &succes);
			if (!succes)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " 
					<< type << "\n" << infoLog 
					<< "\n -- --------------------------------------------------- -- " 
					<< std::endl;
			}
		}
	}
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}
