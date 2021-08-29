#include "chunk_height.hpp"

Pool*	ChunkHeight::pool = nullptr;

void ChunkHeight::initPool(uint32_t pool_size)
{
	if (pool == nullptr) {
		pool = new Pool(pool_size, sizeof(ChunkHeight));
	}
}

void ChunkHeight::destroyPool()
{
	if (pool != nullptr) {
		delete pool;
	}
}