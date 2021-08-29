#pragma once

// stdlib
#include <stdint.h>
#include <cstring>
#include <stdio.h>

// internal
#include "../world/chunk.hpp"
#include "../../util/pool.hpp"

// sizeof(ServerChunk) == 12

struct ServerChunk
{
	Chunk* 			chunk;
	ServerChunk* 	prev;
	ServerChunk* 	next;

	inline void init()
	{
		memset(this, 0, sizeof(ServerChunk));
	}

	static Pool* pool;

	static void initPool(uint32_t size);
	static void destroyPool();

	static inline ServerChunk* alloc_chunk()
	{
	#ifdef DEBUG
		if (pool == nullptr) {
			printf("ERROR : initPool() has not been called for ServerChunk\n");
		}
	#endif
		return (ServerChunk*)pool->p_alloc();
	}

	static inline void free_chunk(ServerChunk* chunk)
	{
		pool->p_free(chunk);
	}
};

class ServerChunkList
{
private:
	ServerChunk* first;
	ServerChunk* last;
public:
	inline ServerChunkList()
	{
		first 	= ServerChunk::alloc_chunk();
		last	= ServerChunk::alloc_chunk();

		first->init();
		last->init();

		first->next = last;
		last->prev = first;
	}

	inline ~ServerChunkList()
	{
		for (ServerChunk* chunk = first->next; chunk != nullptr; chunk = chunk->next)
		{
			ServerChunk::free_chunk(chunk->prev);
		}
		ServerChunk::free_chunk(last);
	}

	inline void clear()
	{
		ServerChunk* chunk = first->next;
		while (chunk != last) {
			chunk = chunk->next;
			ServerChunk::free_chunk(chunk->prev);
		}
		first->next = last;
		last->prev = first;
	}

	inline void addChunk(Chunk* chunk)
	{
		ServerChunk* schunk = ServerChunk::alloc_chunk();
		schunk->init();
		schunk->chunk = chunk;
		schunk->prev = last->prev;
		schunk->prev->next = schunk;
		schunk->next = last;
		schunk->next->prev = schunk;
	}

	void remove(ServerChunk* chunk)
	{
		chunk->prev->next = chunk->next;
		chunk->next->prev = chunk->prev;
		ServerChunk::free_chunk(chunk);
	}

	inline ServerChunk* begin()
	{
		return first->next;
	}

	inline ServerChunk* end()
	{
		return last;
	}
};