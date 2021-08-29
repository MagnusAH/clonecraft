#pragma once

#include <stdint.h>
#include <GL/gl3w.h>

#include "types.hpp"
#include "shader.hpp"
#include "../math/matrix4f.hpp"
#include "../game/player.hpp"
#include "../window/window.hpp"

class Renderer {
private:
	static GLuint vao;
	static GLuint indices_vbo;
	static Shader shader;

	static GLuint loc_projection;
	static GLuint loc_view;
	static GLuint loc_model;

public:
	static Matrix4f projectionMatrix;

	static void init();
	static void setProjection(Window* window, float fov, float near, float far);
	static void drawClientChunks();
	static void bindIndices();
	static void destroy();
};