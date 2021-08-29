#pragma once

#include "world/world.hpp"

#include "world/chunk.hpp"
#include "world/chunk_map.hpp"

#include "world/chunk_height.hpp"
#include "world/height_map.hpp"

#include "client/client_chunk.hpp"
#include "server/server_chunk.hpp"


#ifndef CHUNK_POOL_SIZE
#define CHUNK_POOL_SIZE	(33 * 33 * 33)
#endif

#ifndef CHUNK_ENTRY_POOL_SIZE
#define CHUNK_ENTRY_POOL_SIZE (33 * 33 * 33)
#endif

#ifndef HEIGHT_POOL_SIZE
#define HEIGHT_POOL_SIZE (33 * 33)
#endif

#ifndef HEIGHT_ENTRY_POOL_SIZE
#define HEIGHT_ENTRY_POOL_SIZE (33 * 33)
#endif

#ifndef CLIENT_POOL_SIZE
#define CLIENT_POOL_SIZE (33 * 33 * 33)
#endif

#ifndef SERVER_POOL_SIZE
#define SERVER_POOL_SIZE (33 * 33 * 33)
#endif

// takes ~4GB when pools are full with default sizes
inline void initGamePools()
{
	Chunk::initPool(CHUNK_POOL_SIZE);
	ChunkEntry::initPool(CHUNK_ENTRY_POOL_SIZE);

	ChunkHeight::initPool(HEIGHT_POOL_SIZE);
	HeightEntry::initPool(HEIGHT_ENTRY_POOL_SIZE);

	ClientChunk::initPool(CLIENT_POOL_SIZE);
	ServerChunk::initPool(SERVER_POOL_SIZE);

	World::init();
}