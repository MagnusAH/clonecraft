#include "height_map.hpp"

#define cast_uint32(x) ( *((uint32_t*)(&x)) )

// =========== //
// HeightEntry //
// =========== //

Pool*	HeightEntry::pool = nullptr;

void HeightEntry::initPool(uint32_t pool_size)
{
	if (pool == nullptr) {
		pool = new Pool(pool_size, sizeof(HeightEntry));
	}
}

void HeightEntry::destroyPool()
{
	if (pool != nullptr) {
		delete pool;
	}
}

// ========= //
// HeightMap //
// ========= //

HeightMap::HeightMap()
{
	memset(buckets, 0, sizeof(HeightEntry*) * HEIGHTMAP_SIZE_2);
}

inline uint32_t getIndex(int32_t x, int32_t z)
{
	return (cast_uint32(x) % HEIGHTMAP_SIZE) + ((cast_uint32(z) % HEIGHTMAP_SIZE) * HEIGHTMAP_SIZE);
}

// no overwrite
void HeightMap::put(ChunkHeight* chunk)
{
	uint32_t index = getIndex(chunk->x, chunk->z);
	HeightEntry* entry = (HeightEntry*)HeightEntry::alloc_entry();
	entry->init(chunk);

	HeightEntry* current = buckets[index];
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

void HeightMap::remove(ChunkHeight* chunk)
{
	uint32_t index = getIndex(chunk->x, chunk->z);

	HeightEntry* current = buckets[index];
	if (current == nullptr) {
		return;
	}
	if (current->chunk == chunk) {
		buckets[index] = current->link;
		HeightEntry::free_entry(current);
		return;
	}
	HeightEntry* next = current->link;
	while(next != nullptr)
	{
		if (next->chunk == chunk) {
			current->link = next->link;
			HeightEntry::free_entry(next);
			return;
		}
		current = next;
		next = current->link;
	}
}

void HeightMap::remove(int32_t chunk_x, int32_t chunk_z)
{
	uint32_t index = getIndex(chunk_x, chunk_z);

	HeightEntry* current = buckets[index];
	if (current == nullptr) {
		return;
	}
	if (current->chunk->x == chunk_x && current->chunk->z == chunk_z) {
		buckets[index] = current->link;
		HeightEntry::free_entry(current);
		return;
	}
	HeightEntry* next = current->link;
	while (next != nullptr)
	{
		if (next->chunk->x == chunk_x && next->chunk->z == chunk_z) {
			current->link = next->link;
			HeightEntry::free_entry(next);
			return;
		}
		current = next;
		next = current->link;
	}
}

ChunkHeight* HeightMap::get(int32_t chunk_x, int32_t chunk_z)
{
	uint32_t index = getIndex(chunk_x, chunk_z);

	HeightEntry* current = buckets[index];
	if (current == nullptr) {
		return nullptr;
	}
	if (current->chunk->x == chunk_x && current->chunk->z == chunk_z) {
		return current->chunk;
	}
	while (current->link != nullptr)
	{
		current = current->link;
		if (current->chunk->x == chunk_x && current->chunk->z == chunk_z) {
			return current->chunk;
		}
	}
	return nullptr;
}

void HeightMap::clear()
{
	for (int i = 0; i < HEIGHTMAP_SIZE_2; i++)
	{
		HeightEntry* entry = buckets[i];
		if (entry == nullptr) {
			continue;
		}
		while (entry->link != nullptr) {
			HeightEntry* next = entry->link;
			HeightEntry::free_entry(entry);
			entry = next;
		}
		HeightEntry::free_entry(entry);
	}
	memset(buckets, 0, sizeof(HeightEntry*) * HEIGHTMAP_SIZE_2);
}