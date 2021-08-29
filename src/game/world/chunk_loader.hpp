#pragma once

#include "chunk.hpp"
#include "chunk_height.hpp"

class ChunkLoader
{
private:
	static void generateChunk(Chunk* chunk);
	static void generateHeight(ChunkHeight* chunk_height);
public:
	static void load(Chunk* chunk);
	static void load(ChunkHeight* chunk_height);

	static void saveAndFree(Chunk* chunk);
};