#pragma once

// stdlib
#include <stdint.h>
#include <stdio.h>

// internal
#include "block.hpp"
#include "../../util/pool.hpp"

// config
#define CHUNK_SIZE		32
#define CHUNK_SIZE_2	(CHUNK_SIZE * CHUNK_SIZE)
#define CHUNK_SIZE_3	(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE)

// sizeof(Chunk) == ~ 128 KB

struct Chunk
{
	int32_t 	x;
	int32_t		y;
	int32_t		z;
	int32_t 	loaded;
	int32_t 	score;
	Block 		block[CHUNK_SIZE_3];

	inline void init(const int32_t chunk_x, const int32_t chunk_y, const int32_t chunk_z)
	{
		x 		= chunk_x;
		y 		= chunk_y;
		z 		= chunk_z;
		score 	= 0;
		loaded 	= false;
	}

	static Pool* pool;

	static void initPool(uint32_t size);
	static void destroyPool();

	static inline Chunk* alloc_chunk()
	{
	#ifdef DEBUG
		if (pool == nullptr) {
			printf("ERROR : initPool() has not been called for Chunk\n");
		}
	#endif
		return (Chunk*)pool->p_alloc();
	}

	static inline void free_chunk(Chunk* chunk)
	{
		pool->p_free(chunk);
	}
};