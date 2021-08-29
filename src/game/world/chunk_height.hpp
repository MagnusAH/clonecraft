#pragma once

// stdlib
#include <stdint.h>
#include <stdio.h>

// internal
#include "chunk.hpp"
#include "../../util/pool.hpp"

// sizeof(ChunkHeight) == 16 + 4 * CHUNK_SIZE_2 == 4112 ~ 4 KB

struct ChunkHeight
{
	int32_t		x;
	int32_t		z;
	uint32_t	refs;
	int32_t		loaded;
	int32_t		height[CHUNK_SIZE_2];

	inline void init(const int32_t chunk_x, const int32_t chunk_z)
	{
		x		= chunk_x;
		z		= chunk_z;
		refs	= 1;
		loaded	= false;
	}

	static Pool* pool;

	static void initPool(uint32_t size);
	static void destroyPool();

	static inline ChunkHeight* alloc_chunk()
	{
	#ifdef DEBUG
		if (pool == nullptr) {
			printf("ERROR : initPool() has not been called for ChunkHeight\n");
		}
	#endif
		return (ChunkHeight*)pool->p_alloc();
	}

	static inline void free_chunk(ChunkHeight* chunk)
	{
		pool->p_free(chunk);
	}
};