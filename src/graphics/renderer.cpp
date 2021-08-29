#include "renderer.hpp"

#include "../game/client/client.hpp"
#include "../math/frustum.hpp"
#include "../util/io.hpp"

GLuint 		Renderer::vao;
Matrix4f	Renderer::projectionMatrix;
Shader 		Renderer::shader;

GLuint 		Renderer::loc_projection;
GLuint 		Renderer::loc_view;
GLuint 		Renderer::loc_model;

GLuint		Renderer::indices_vbo;

void Renderer::init()
{
	// vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0); // vertex
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);	// 3x4 bytes
	glVertexAttribBinding(0, 0);

	glEnableVertexAttribArray(1); // color
	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 12); // 3x4 bytes
	glVertexAttribBinding(1, 0);

	glEnableVertexAttribArray(2); // texCoord
	glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 24);
	glVertexAttribBinding(2, 0);

	glBindVertexArray(0);

	// shader
	std::string vert_code = readFile("shaders/block_vert.glsl");
	std::string frag_code = readFile("shaders/block_frag.glsl");

	shader.attachShader(GL_VERTEX_SHADER, vert_code.c_str());
	shader.attachShader(GL_FRAGMENT_SHADER, frag_code.c_str());

	shader.linkShaders();

	loc_model 		= shader.getUniform("modelMatrix");
	loc_projection	= shader.getUniform("projectionMatrix");
	loc_view 		= shader.getUniform("viewMatrix");

	GLuint* indices = (GLuint*)malloc(sizeof(GLuint) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 3 * 6);
	for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 3 * 6; i+=6)
	{
		int base = (i * 4) / 6;
		indices[i+0] 	= base+0;
		indices[i+1] 	= base+1;
		indices[i+2] 	= base+2;
		indices[i+3] 	= base+2;
		indices[i+4] 	= base+3;
		indices[i+5] 	= base+0;
	}

	glGenBuffers(1, &indices_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 3 * 6, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	free(indices);
}

void Renderer::bindIndices()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo);
}

void Renderer::setProjection(Window* window, float fov, float near_v, float far_v)
{
	float aspect = (float)window->frameWidth() / (float)window->frameHeight();
	projectionMatrix.setProjection(aspect, fov, near_v, far_v);
}

void Renderer::drawClientChunks()
{
	Frustum frust;
	frust.setCamera(Client::player);

	glUseProgram(shader.m_program);
	glBindVertexArray(vao);
	{
		glUniformMatrix4fv(loc_projection, 1, GL_FALSE, projectionMatrix.mat);
		glUniformMatrix4fv(loc_view, 1, GL_FALSE, Client::player->getViewMatrix().mat);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo);

		for (uint32_t i = 0; i < Client::getChunkCount(); i++)
		{
			ClientChunk* cl_chunk = Client::client_chunks[i];

			float rx = (cl_chunk->chunk->x * CHUNK_SIZE);
			float ry = (cl_chunk->chunk->y * CHUNK_SIZE);
			float rz = (cl_chunk->chunk->z * CHUNK_SIZE);

			Matrix4f modelMatrix;
			modelMatrix.translate(rx, ry, rz);

			// use bounding spheres for chunks
			if (!frust.inside(Vector3f(rx, ry, rz), 0.867f * CHUNK_SIZE)) {
				continue;
			}

			glUniformMatrix4fv(Renderer::loc_model, 1, GL_FALSE, modelMatrix.mat);

			glBindVertexBuffer(0, cl_chunk->vbo_vertices, 0, sizeof(VertexPNT));
			glDrawElements(GL_TRIANGLES, cl_chunk->indice_count, GL_UNSIGNED_INT, 0);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);
	glUseProgram(0);
}

void Renderer::destroy()
{

}