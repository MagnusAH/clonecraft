#include "chunk_loader.hpp"

// external
#include <stb/stb_perlin.h>

// internal
#include "world.hpp"

void ChunkLoader::load(ChunkHeight* chunk_height)
{
	generateHeight(chunk_height);
}

void ChunkLoader::load(Chunk* chunk)
{
	generateChunk(chunk);
}

void ChunkLoader::generateHeight(ChunkHeight* chunk_height)
{
	int32_t off_x = chunk_height->x * CHUNK_SIZE;
	int32_t off_z = chunk_height->z * CHUNK_SIZE;
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int z = 0; z < CHUNK_SIZE; z++)
		{
			float rval = stb_perlin_turbulence_noise3((float)(x + off_x) * World::scale, World::seed, (float)(z + off_z) * World::scale,
													  2.2f, 0.5f, 6);
			chunk_height->height[x + z * CHUNK_SIZE] = rval * (World::terrainTop - World::terrainBottom) + World::terrainBottom;
		}
	}

	chunk_height->loaded = true;
}

void ChunkLoader::generateChunk(Chunk* chunk)
{
	ChunkHeight* height_data = World::getChunkHeight(chunk->x, chunk->z);
	int32_t off_y = chunk->y * CHUNK_SIZE;

	for (int z = 0; z < CHUNK_SIZE; z++)
	{
		int iz = z * CHUNK_SIZE;
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			int izy = (iz + y) * CHUNK_SIZE;
			for (int x = 0; x < CHUNK_SIZE; x++)
			{
				int index = izy + x;
				int height = height_data->height[iz + x];
				int ry = y + off_y;
				// TODO rearange into 4 for loops
				if (ry < height - 4) {
					chunk->block[index].id = BlockTypes::STONE;
				} else if (ry < height) {
					chunk->block[index].id = BlockTypes::DIRT;
				} else if (ry == height) {
					chunk->block[index].id = BlockTypes::GRASS;
				} else {
					chunk->block[index].id = BlockTypes::AIR;
				}
			}
		}
	}

	chunk->loaded = true;
}

void ChunkLoader::saveAndFree(Chunk* chunk)
{
	Chunk::free_chunk(chunk);
}