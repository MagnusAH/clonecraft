#include "shader.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

Shader::Shader()
{
	m_program = 0;
}

Shader::~Shader()
{
	deleteProgram();
}


bool checkCompile(GLuint shader);
bool checkLink(GLuint program);

bool Shader::attachShader(GLenum type, const char* code)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);
	if (!checkCompile(shader)) {
		glDeleteShader(shader);
		return false;
	}
	m_shaders.push_back(shader);
	return true;
}

bool Shader::linkShaders()
{
	GLuint program = glCreateProgram();
	for (uint64_t i = 0; i < m_shaders.size(); i++)
	{
		glAttachShader(program, m_shaders[i]);
	}
	glLinkProgram(program);
	if (!checkLink(program)) {
		glDeleteProgram(program);
		return false;
	}
	for (uint64_t i = 0; i < m_shaders.size(); i++)
	{
		glDetachShader(program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}
	m_shaders.clear();
	m_shaders.shrink_to_fit();
	m_program = program;
	return true;
}

void Shader::deleteProgram()
{
	glDeleteProgram(m_program);
}

bool checkCompile(GLuint shader)
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status) {
		GLint log_size;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
		char* log = (char*)malloc(log_size);
		glGetShaderInfoLog(shader, log_size, NULL, log);
		printf(log);
		free(log);
		return false;
	}
	return true;
}

bool checkLink(GLuint program)
{
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status) {
		GLint log_size;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
		char* log = (char*)malloc(log_size);
		glGetProgramInfoLog(program, log_size, NULL, log);
		printf(log);
		free(log);
		return false;
	}
	return true;
}