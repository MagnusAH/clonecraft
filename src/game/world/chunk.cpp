#include "chunk.hpp"

Pool*	Chunk::pool = nullptr;

void Chunk::initPool(uint32_t pool_size)
{
	if (pool == nullptr) {
		pool = new Pool(pool_size, sizeof(Chunk));
	}
}

void Chunk::destroyPool()
{
	if (pool != nullptr) {
		delete pool;
	}
}