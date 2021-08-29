#include "client_chunk.hpp"

Pool*	ClientChunk::pool = nullptr;

void ClientChunk::initPool(uint32_t pool_size)
{
	if (pool == nullptr) {
		pool = new Pool(pool_size, sizeof(ClientChunk));
	}
}

void ClientChunk::destroyPool()
{
	if (pool != nullptr) {
		delete pool;
	}
}