#pragma once

// stdlib
#include <stdint.h>
#include <cmath>

// internal
#include "chunk.hpp"
#include "chunk_map.hpp"

#include "chunk_height.hpp"
#include "height_map.hpp"

class World
{
private:
	static HeightMap* heights;
public:
	static ChunkMap* chunks;

	static float seed;
	static float scale;

	static int32_t terrainBottom;
	static int32_t terrainTop;

	static void init();
	static void destroy();

	static Chunk* loadChunk(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z);
	static void unloadChunk(Chunk* chunk);

	static inline Chunk* getChunk(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z)
	{
		return chunks->get(chunk_x, chunk_y, chunk_z);
	}

	static inline ChunkHeight* getChunkHeight(int32_t chunk_x, int32_t chunk_z)
	{
		ChunkHeight* height =  heights->get(chunk_x, chunk_z);
		return height;
	}

	static inline int32_t toChunkScale(float val)
	{
		return floor(val / CHUNK_SIZE + 0.5f);
	}
};