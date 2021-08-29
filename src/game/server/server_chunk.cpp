#include "server_chunk.hpp"

Pool*	ServerChunk::pool = nullptr;

void ServerChunk::initPool(uint32_t pool_size)
{
	if (pool == nullptr) {
		pool = new Pool(pool_size, sizeof(ServerChunk));
	}
}

void ServerChunk::destroyPool()
{
	if (pool != nullptr) {
		delete pool;
	}
}