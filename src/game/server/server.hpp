#pragma once

#include "server_chunk.hpp"
#include "server_player.hpp"

// #include "../player.hpp"

// remove chunk_list and iterate through World::chunks instead, server_chunks no longer have a purpose after moving score tracking to regular chunks
class Server
{
private:
	static ServerChunkList* chunk_list;
	static uint64_t			tick;
	// static uint32_t			player_count;

	static inline Chunk* safeGet(int32_t x, int32_t y, int32_t z)
	{
		Chunk* chunk = World::getChunk(x, y, z);
		if (chunk == nullptr) {
			chunk = World::loadChunk(x, y, z);
			chunk_list->addChunk(chunk);
		}
		return chunk;
	}

public:
	static ServerPlayer* 	players;
	static int32_t			load_radius;

	static void init();
	static void destroy();

	static void update();

	static void descoreChunk(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z);
	static Chunk* requestChunk(uint32_t id, int32_t chunk_x, int32_t chunk_y, int32_t chunk_z);

	static uint32_t addPlayer();
	static void removePlayer(uint32_t id);
};