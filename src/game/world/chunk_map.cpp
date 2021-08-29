#include "chunk_map.hpp"

#define cast_uint32(x) ( *((uint32_t*)(&x)) )

// ========== //
// ChunkEntry //
// ========== //

Pool*	ChunkEntry::pool = nullptr;

void ChunkEntry::initPool(uint32_t pool_size)
{
	if (pool == nullptr) {
		pool = new Pool(pool_size, sizeof(ChunkEntry));
	}
}

void ChunkEntry::destroyPool()
{
	if (pool != nullptr) {
		delete pool;
	}
}

// ======== //
// ChunkMap //
// ======== //

ChunkMap::ChunkMap()
{
	memset(buckets, 0, sizeof(ChunkEntry*) * CHUNKMAP_SIZE_3);
}

inline uint32_t getIndex(int32_t x, int32_t y, int32_t z)
{
	return (cast_uint32(x) % CHUNKMAP_SIZE) + ((cast_uint32(y) % CHUNKMAP_SIZE) * CHUNKMAP_SIZE) + ((cast_uint32(z) % CHUNKMAP_SIZE) * CHUNKMAP_SIZE_2);
}

// no overwrite
void ChunkMap::put(Chunk* chunk)
{
	uint32_t index = getIndex(chunk->x, chunk->y, chunk->z);
	ChunkEntry* entry = (ChunkEntry*)ChunkEntry::alloc_entry();
	entry->init(chunk);

	ChunkEntry* current = buckets[index];
	if (current == nullptr) {
		buckets[index] = entry;
		return;
	}
	while (current->link != nullptr)
	{
		current = current->link;
	}
	current->link = entry;
}

void ChunkMap::remove(Chunk* chunk)
{
	uint32_t index = getIndex(chunk->x, chunk->y, chunk->z);

	ChunkEntry* current = buckets[index];
	if (current == nullptr) {
		return;
	}
	if (current->chunk == chunk) {
		buckets[index] = current->link;
		ChunkEntry::free_entry(current);
		return;
	}
	ChunkEntry* next = current->link;
	while(next != nullptr)
	{
		if (next->chunk == chunk) {
			current->link = next->link;
			ChunkEntry::free_entry(next);
			return;
		}
		current = next;
		next = current->link;
	}
}

void ChunkMap::remove(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z)
{
	uint32_t index = getIndex(chunk_x, chunk_y, chunk_z);

	ChunkEntry* current = buckets[index];
	if (current == nullptr) {
		return;
	}
	if (current->chunk->x == chunk_x && current->chunk->y == chunk_y && current->chunk->z == chunk_z) {
		buckets[index] = current->link;
		ChunkEntry::free_entry(current);
		return;
	}
	ChunkEntry* next = current->link;
	while (next != nullptr)
	{
		if (next->chunk->x == chunk_x && next->chunk->y == chunk_y && next->chunk->z == chunk_z) {
			current->link = next->link;
			ChunkEntry::free_entry(next);
			return;
		}
		current = next;
		next = current->link;
	}
}

Chunk* ChunkMap::get(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z)
{
	uint32_t index = getIndex(chunk_x, chunk_y, chunk_z);

	ChunkEntry* current = buckets[index];
	if (current == nullptr) {
		return nullptr;
	}
	if (current->chunk->x == chunk_x && current->chunk->y == chunk_y && current->chunk->z == chunk_z) {
		return current->chunk;
	}
	while (current->link != nullptr)
	{
		current = current->link;
		if (current->chunk->x == chunk_x && current->chunk->y == chunk_y && current->chunk->z == chunk_z) {
			return current->chunk;
		}
	}
	return nullptr;
}

void ChunkMap::clear()
{
	for (int i = 0; i < CHUNKMAP_SIZE_3; i++)
	{
		ChunkEntry* entry = buckets[i];
		if (entry == nullptr) {
			continue;
		}
		while (entry->link != nullptr) {
			ChunkEntry* next = entry->link;
			ChunkEntry::free_entry(entry);
			entry = next;
		}
		ChunkEntry::free_entry(entry);
	}
	memset(buckets, 0, sizeof(ChunkEntry*) * CHUNKMAP_SIZE_3);
}

ChunkEntry* ChunkMap::firstBucket()
{
	uint32_t index = 0;
	while (index < CHUNKMAP_SIZE_3) {
		if (buckets[index] != nullptr) {
			return buckets[index];
		}
		index++;
	}
	return nullptr;
}

ChunkEntry* ChunkMap::nextBucket(ChunkEntry* bucket)
{
	uint32_t index = getIndex(bucket->chunk->x, bucket->chunk->y, bucket->chunk->z);
	while (++index < CHUNKMAP_SIZE_3) {
		if (buckets[index] != nullptr) {
			return buckets[index];
		}
	}
	return nullptr;
}