#include "world.hpp"
#include "chunk_loader.hpp"

ChunkMap*	World::chunks;
HeightMap*	World::heights;

float		World::seed				= 14.698f;
float		World::scale			= 0.0037f;

int32_t		World::terrainBottom 	= -48;
int32_t		World::terrainTop		= 48;

void World::init()
{
	if (chunks == nullptr) {
		chunks = new ChunkMap();
		heights = new HeightMap();
	} else {
		chunks->clear();
		heights->clear();
	}
}

void World::destroy()
{
	if (chunks != nullptr) {
		delete chunks;
		delete heights;
	}
}

Chunk* World::loadChunk(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z)
{
	Chunk* chunk = chunks->createChunk(chunk_x, chunk_y, chunk_z);

	// store generated heights so that they only need to be computed once for every column
	ChunkHeight* chunk_height = heights->get(chunk->x, chunk->z);
	if (chunk_height == nullptr) {
		chunk_height = heights->createChunk(chunk_x, chunk_z);
		ChunkLoader::load(chunk_height);	/* generates chunk heights */
	} else {
		chunk_height->refs = chunk_height->refs + 1;
	}
	ChunkLoader::load(chunk); /* tries to load chunk from file, if not found generates chunk instead */
	return chunk;
}

void World::unloadChunk(Chunk* chunk)
{
	chunks->remove(chunk);

	// remove generated heights if there are no chunks in the column
	ChunkHeight* chunk_height = heights->get(chunk->x, chunk->z);
	if (chunk_height->refs == 1) {
		heights->remove(chunk_height);
	} else {
		chunk_height->refs = chunk_height->refs - 1;
	}
	ChunkLoader::saveAndFree(chunk);	/* saves chunk to disk and moves chunk object to the pool */
}