#pragma once

// stdlib
#include <cstring>
#include <stdio.h>

// external
#include <GL/gl3w.h>

// internal
#include "../world/chunk.hpp"
#include "../../util/pool.hpp"

// sizeof(ClientChunk) == 16
struct ClientChunk
{
	Chunk* 		chunk;
	GLuint 		vbo_vertices;	// if 0, not loading : if not 0, currently loading
	GLuint 		indice_count;	// if 0, not loaded	 : if not 0, loaded

	inline void init()
	{
		memset(this, 0, sizeof(ClientChunk));
	}

	inline void destroy()
	{
		if (vbo_vertices != 0) {
			glDeleteBuffers(1, &vbo_vertices);
		}
	}

	static Pool* pool;

	static void initPool(uint32_t size);
	static void destroyPool();

	static inline ClientChunk* alloc_chunk()
	{
	#ifdef DEBUG
		if (pool == nullptr) {
			printf("ERROR : initPool() has not been called for ClientChunk\n");
		}
	#endif
		return (ClientChunk*)pool->p_alloc();
	}
	static inline void free_chunk(ClientChunk* chunk)
	{
		pool->p_free(chunk);
	}
};