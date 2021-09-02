#pragma once

// this is an entire file is a mess and temporary because i'm lazy

#include "../graphics/types.hpp"
#include <GL/gl3w.h>
#include <vector>

#include "../math/matrix4f.hpp"
#include "../math/vector4f.hpp"
#include "../math/vector3f.hpp"

#include <stdio.h>

struct Model
{
	GLuint vertice_vbo;
	GLuint indice_vbo;
	GLuint indice_count;

	inline void load(VertexPNT* vertices, GLuint v_count, GLuint* indices, GLuint i_count)
	{
		// vertice vbo
		glGenBuffers(1, &vertice_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vertice_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPNT) * v_count, vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// indices vbo
		// glGenBuffers(1, &indice_vbo);
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indice_vbo);
		// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * i_count, indices, GL_STATIC_DRAW);
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		indice_count = v_count * 6 / 4;
	}

	inline ~Model()
	{

	}

	inline void draw()
	{
		glBindVertexBuffer(0, vertice_vbo, 0, sizeof(VertexPNT));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indice_vbo);
		glDrawElements(GL_TRIANGLES, indice_count, GL_UNSIGNED_INT, 0);
	}
};

struct Mesh
{
	std::vector<VertexPNT> vertices;
	std::vector<GLuint> indices;

	inline Model createModel()
	{
		Model model;
		model.load(&vertices[0], vertices.size(), &indices[0], indices.size());
		return model;
	}

	inline void createVBO(GLuint* ind_vbo, GLuint* vert_vbo)
	{
		// vertices vbo
		glGenBuffers(1, vert_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, *vert_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPNT)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// indices vbo
		// glGenBuffers(1, ind_vbo);
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ind_vbo);
		// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	inline void generate(int* heights, int width, int length)
	{
		for (int x = 1; x < width-1; x++) {
			for (int z = 1; z < length-1; z++) {
				int y = heights[z*width + x];
				pushTop(x, y, z);
			}
		}
	}

	inline void pushQuad(VertexPNT v0, VertexPNT v1, VertexPNT v2, VertexPNT v3)
	{
		// GLuint base = vertices.size();
		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);

		// indices.push_back(base);
		// indices.push_back(base+1);
		// indices.push_back(base+2);
		// indices.push_back(base+2);
		// indices.push_back(base+3);
		// indices.push_back(base);
	}

	inline void pushDir(int textureID, Vector3f p0, Vector3f p1, Vector3f p2, Vector3f p3, float x, float y, float z, float nx, float ny, float nz)
	{
		const int atlasWidth = 1;
		const int atlasHeight = 4;

		const float widthScale = 1.0f / (float)atlasWidth;
		const float heightScale = 1.0f / (float)atlasHeight;

		float min_x = (textureID % atlasWidth) * widthScale;
		float max_x = min_x + widthScale;
		float min_y = (textureID / atlasWidth) * heightScale;
		float max_y = min_y + heightScale;

		Vector3f
			normal(nx, ny, nz),
			pos(x, y, z);

		Vector2f
			t0(min_x, max_y), // 0, 1
			t1(max_x, max_y), // 1, 1
			t2(max_x, min_y), // 1, 0
			t3(min_x, min_y); // 0, 0

		VertexPNT
			v0(pos+p0, normal, t0),
			v1(pos+p1, normal, t1),
			v2(pos+p2, normal, t2),
			v3(pos+p3, normal, t3);

		pushQuad(v0, v1, v2, v3);
	}

	inline void pushFront(float x, float y, float z)
	{
		Vector3f
			p0(-0.5f, -0.5f, 0.5f),
			p1(0.5f, -0.5f, 0.5f),
			p2(0.5f, 0.5f, 0.5f),
			p3(-0.5f, 0.5f, 0.5f);

		pushDir(1, p0, p1, p2, p3, x, y, z, 0, 0, 1);
	}

	inline void pushBack(float x, float y, float z)
	{
		Vector3f
			p0(0.5f, -0.5f, -0.5f),
			p1(-0.5f, -0.5f, -0.5f),
			p2(-0.5f, 0.5f, -0.5f),
			p3(0.5f, 0.5f, -0.5f);

		pushDir(1, p0, p1, p2, p3, x, y, z, 0, 0, -1);
	}


	inline void pushLeft(float x, float y, float z)
	{
		Vector3f
			p0(-0.5f, -0.5f, -0.5f),
			p1(-0.5f, -0.5f, 0.5f),
			p2(-0.5f, 0.5f, 0.5f),
			p3(-0.5f, 0.5f, -0.5f);

		pushDir(1, p0, p1, p2, p3, x, y, z, -1, 0, 0);
	}


	inline void pushRight(float x, float y, float z)
	{
		Vector3f
			p0(0.5f, -0.5f, 0.5f),
			p1(0.5f, -0.5f, -0.5f),
			p2(0.5f, 0.5f, -0.5f),
			p3(0.5f, 0.5f, 0.5f);

		pushDir(1, p0, p1, p2, p3, x, y, z, 1, 0, 0);
	}


	inline void pushTop(float x, float y, float z)
	{
		Vector3f
			p0(-0.5f, 0.5f, 0.5f),
			p1(0.5f, 0.5f, 0.5f),
			p2(0.5f, 0.5f, -0.5f),
			p3(-0.5f, 0.5f, -0.5f);

		pushDir(2, p0, p1, p2, p3, x, y, z, 0, 1, 0);
	}


	inline void pushBottom(float x, float y, float z)
	{
		Vector3f
			p0(-0.5f, -0.5f, -0.5f),
			p1(0.5f, -0.5f, -0.5f),
			p2(0.5f, -0.5f, 0.5f),
			p3(-0.5f, -0.5f, 0.5f);

		pushDir(0, p0, p1, p2, p3, x, y, z, 0, -1, 0);
	}
};