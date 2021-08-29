#pragma once

#include "client_chunk.hpp"

#include "../player.hpp"
#include "../world/world.hpp"

class Client
{
private:
	static uint32_t radius;
	static uint32_t diameter;
	static uint32_t chunk_count;
public:
	static Player* player;
	static ClientChunk** client_chunks;

	static void init();
	static void destroy();

	static void connect();

	static Chunk* getChunk(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z);

	static void	setViewDistance(uint32_t distance);
	static inline uint32_t getViewDistance() { return radius; }
	static inline uint32_t getViewDiameter() { return diameter; }
	static inline uint32_t getChunkCount() { return chunk_count; }

	static void sync();
	static void update();

	static inline void updatePlayer() { player->update(); }
};