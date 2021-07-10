#pragma once
#ifndef SHADER_H
#define SHADER_H

// Compiled lib and header
#include <glad/glad.h>
#include <glm/glm.hpp>

// Core cpp lib
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class Shader
{
private:

	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileError(GLuint&, std::string&&) const;

public:
	// The program id
	GLuint ID;

	// Default constructor
	Shader() = default;

	// Constructor reads and builds the shader
	explicit Shader(const char*, const char*, const char* = nullptr);

	// Deactivate copy assignment operator
	Shader(Shader&) = delete;
	Shader operator=(Shader&) = delete;

	// Deactive move assignment operator
	Shader(Shader&&) = delete;
	Shader operator=(Shader&&) = delete;

	// use/activate the shader
	void use();

	// utility uniform function
	void setInt(const GLchar*, GLint) const;
	void setFloat(const GLchar*, GLfloat) const;
	void setVec2(const GLchar*, const glm::vec2&) const;
	void setVec2(const GLchar*, float, float) const;
	void setVec3(const GLchar*, const glm::vec3&) const;
	void setVec3(const GLchar*, float, float, float) const;
	void setVec4(const GLchar*, const glm::vec4&) const;
	void setVec4(const GLchar*, float, float, float, float) const;
	void setMat2(const GLchar*, const glm::mat2&) const;
	void setMat3(const GLchar*, const glm::mat3&) const;
	void setMat4(const GLchar*, const glm::mat4&) const;

	// Default destructor
	~Shader();
};

#endif // !SHADER_H

