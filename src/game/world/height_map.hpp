#pragma once

// stdlib
#include <cstring>
#include <stdio.h>

// internal
#include "chunk_height.hpp"
#include "../../util/pool.hpp"

#define HEIGHTMAP_SIZE 		67
#define HEIGHTMAP_SIZE_2	(HEIGHTMAP_SIZE * HEIGHTMAP_SIZE)

// sizeof(HeightEntry) == 32
struct HeightEntry
{
	ChunkHeight* chunk;
	HeightEntry* link;

	inline void init(ChunkHeight* i_chunk)
	{
		chunk 	= i_chunk;
		link	= nullptr;
	}

	static Pool* pool;
	static void initPool(uint32_t pool_size);
	static void destroyPool();

	static inline HeightEntry* alloc_entry()
	{
	#ifdef DEBUG
		if (pool == nullptr) {
			printf("ERROR : initPool() has not been called for HeightEntry\n");
		}
	#endif
		return (HeightEntry*)pool->p_alloc();
	}
	static inline void free_entry(HeightEntry* entry)
	{
		pool->p_free(entry);
	}
};

// like a hash map, but 2D
class HeightMap
{
private:
	HeightEntry* buckets[HEIGHTMAP_SIZE_2];
public:
	HeightMap();

	void put(ChunkHeight* chunk);
	inline ChunkHeight* createChunk(uint32_t chunk_x, uint32_t chunk_z)
	{
		ChunkHeight* chunk = ChunkHeight::alloc_chunk();
		chunk->init(chunk_x, chunk_z);
		put(chunk);
		return chunk;
	}

	void remove(ChunkHeight* chunk);
	void remove(int32_t chunk_x, int32_t chunk_z);

	ChunkHeight* get(int32_t chunk_x, int32_t chunk_z);

	void clear();
};