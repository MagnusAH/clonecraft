#pragma once

// stdlib
#include <cstring>
#include <stdio.h>

// internal
#include "chunk.hpp"
#include "../../util/pool.hpp"

#define CHUNKMAP_SIZE 		67
#define CHUNKMAP_SIZE_2		(CHUNKMAP_SIZE * CHUNKMAP_SIZE)
#define CHUNKMAP_SIZE_3		(CHUNKMAP_SIZE * CHUNKMAP_SIZE * CHUNKMAP_SIZE)

// sizeof(ChunkEntry) == 32
struct ChunkEntry
{
	Chunk* chunk;
	ChunkEntry* link;

	inline void init(Chunk* i_chunk)
	{
		chunk 	= i_chunk;
		link	= nullptr;
	}

	static Pool* pool;
	static void initPool(uint32_t pool_size);
	static void destroyPool();

	static inline ChunkEntry* alloc_entry()
	{
	#ifdef DEBUG
		if (pool == nullptr) {
			printf("ERROR : initPool() has not been called for ChunkEntry\n");
		}
	#endif
		return (ChunkEntry*)pool->p_alloc();
	}
	static inline void free_entry(ChunkEntry* entry)
	{
		pool->p_free(entry);
	}
};

// like a hash map, but 3D
class ChunkMap
{
private:
	ChunkEntry* buckets[CHUNKMAP_SIZE_3];
public:
	ChunkMap();

	void put(Chunk* chunk);
	inline Chunk* createChunk(uint32_t chunk_x, uint32_t chunk_y, uint32_t chunk_z)
	{
		Chunk* chunk = Chunk::alloc_chunk();
		chunk->init(chunk_x, chunk_y, chunk_z);
		put(chunk);
		return chunk;
	}

	void remove(Chunk* chunk);
	void remove(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z);

	// takes ~14-34 ns per retrieval depending on cache (ryzen 5 3600 at ~3.94 GHz, windows 10)
	Chunk* get(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z);

	void clear();

	inline uint32_t getSize() { return CHUNKMAP_SIZE_3; }
	ChunkEntry* firstBucket();
	ChunkEntry* nextBucket(ChunkEntry* bucket);
};