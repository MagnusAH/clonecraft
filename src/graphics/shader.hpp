#pragma once

#include <GL/gl3w.h>
#include <vector>

class Shader
{
private:
	std::vector<GLuint> m_shaders;

public:
	GLint m_program;
	Shader();
	~Shader();
	bool attachShader(GLenum type, const char* code);
	bool linkShaders();
	inline void use()
	{
		glUseProgram(m_program);
	}
	inline GLint getUniform(const char* name)
	{
		return glGetUniformLocation(m_program, name);
	}
	void deleteProgram();
};