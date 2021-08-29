#include "server.hpp"

#include "../world/world.hpp"
#include "../client/client_chunk.hpp"

#include <cmath>

#include <stdio.h>

ServerChunkList*	Server::chunk_list;
ServerPlayer*		Server::players;
int32_t				Server::load_radius;
uint64_t			Server::tick;

void Server::init()
{
	chunk_list = new ServerChunkList();
	players = nullptr;
	tick = 0;
}

void Server::destroy()
{
	/*
	for (player in players) {
		disconnect player;
	}
	*/
	World::destroy();
	delete chunk_list;
}

uint32_t Server::addPlayer()
{
	players = new ServerPlayer();

	for (int z = -load_radius; z <= load_radius; z++)
	{
		for (int y = -load_radius; y <= load_radius; y++)
		{
			for (int x = -load_radius; x <= load_radius; x++)
			{
				Chunk* chunk = World::getChunk(x, y, z);
				if (chunk == nullptr) {
					chunk = World::loadChunk(x, y, z);
				}
				chunk->score++;
			}
		}
	}

	return 0;
}


void Server::removePlayer(uint32_t id)
{
	delete players;
}

void Server::descoreChunk(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z)
{
	Chunk* chunk = World::getChunk(chunk_x, chunk_y, chunk_z);
	if (chunk == nullptr) {
		printf("NULL CHUNK DESCORED => x=%i, y=%i, z=%i\n", chunk_x, chunk_y, chunk_z);
		return;
	}
	chunk->score--;
	if (chunk->score == 0) {
		World::unloadChunk(chunk);
	}
}

Chunk* Server::requestChunk(uint32_t id, int32_t chunk_x, int32_t chunk_y, int32_t chunk_z)
{
	int pcx = World::toChunkScale(players->x);
	int pcy = World::toChunkScale(players->y);
	int pcz = World::toChunkScale(players->z);

	int dcx = chunk_x - pcx;
	int dcy = chunk_y - pcy;
	int dcz = chunk_z - pcz;

	// check if in bounds
	if (abs(dcx) > load_radius || abs(dcy) > load_radius || abs(dcz) > load_radius) {
		return nullptr;
	}

	// get requested chunk and load if required
	Chunk* chunk = World::getChunk(chunk_x, chunk_y, chunk_z);
	if (chunk == nullptr) {
		chunk = World::loadChunk(chunk_x, chunk_y, chunk_z);
	}

	return chunk;
}

void Server::update()
{
	/*
	for (chunk in chunk_list) {
		update chunk
	}
	*/

	/*
	for (player in players)
	*/
	ServerPlayer* player = players;
	{
		int cx = World::toChunkScale(player->x);
		int cy = World::toChunkScale(player->y);
		int cz = World::toChunkScale(player->z);

		int lcx = World::toChunkScale(player->lx);
		int lcy = World::toChunkScale(player->ly);
		int lcz = World::toChunkScale(player->lz);

		int xdir = cx - lcx;
		int ydir = cy - lcy;
		int zdir = cz - lcz;

		// player moved chunks

		if (xdir | ydir | zdir) {
			printf("\n===========================\n"\
				   "=> UPDATED LOADED CHUNKS <=\n"\
				   "===========================\n");
			printf("Server update tick %ju\n", tick);
			for (int z = -load_radius; z <= load_radius; z++)
			{
				int oz = z + lcz;
				int doz = abs(oz - cz);

				int nz = z + cz;
				int dnz = abs(nz - lcz);

				for (int y = -load_radius; y <= load_radius; y++)
				{
					int oy = y + lcy;
					int doy = abs(oy - cy);

					int ny = y + cy;
					int dny = abs(ny - lcy);

					for (int x = -load_radius; x <= load_radius; x++)
					{
						int ox = x + lcx;
						int dox = abs(ox - cx);

						int nx = x + cx;
						int dnx = abs(nx - lcx);

						if (dox > load_radius || doy > load_radius || doz > load_radius) {
							descoreChunk(ox, oy, oz);
						}

						if (dnx > load_radius || dny > load_radius || dnz > load_radius) {
							Chunk* chunk = World::getChunk(nx, ny, nz);
							if (chunk == nullptr) {
								chunk = World::loadChunk(nx, ny, nz);
							}
							chunk->score++;
						}

					}
				}
			}
			uint64_t chunks_available 	= Chunk::pool->poolCount();
			uint64_t chunks_used		= Chunk::pool->inCirculation();
			uint64_t chunks_total		= Chunk::pool->totalCount();
			uint64_t chunks_memory		= chunks_total * sizeof(Chunk) / (1024 * 1024);
			printf("STATS: Chunks\n\t=> %ju chunks in pool\n\t=> %ju chunks in use\n\t=> %ju chunks total using\n\t\t%ju MB of RAM\n",
					chunks_available, chunks_used, chunks_total, chunks_memory);
		}

		player->update();
		tick++;
	}

	for (ChunkEntry* bucket = World::chunks->firstBucket(); bucket != nullptr; bucket = World::chunks->nextBucket(bucket))
	{
		for (ChunkEntry* entry = bucket; entry != nullptr; entry = entry->link)
		{
			// update chunk
		}
	}

	// replace with
	/*
	on player request chunk {
		if 		(cx > pcx) => descore negative x
		else if (cx < pcx) => descore positive x
		if		(cy > pcy) => descore negative y
		else if (cy < pcy) => descore positive y
		if		(cz > pcz) => descore negative z
		else if (cz < pcz) => descore positive z
	}

	on chunk descore {
		if (chunk->score == 0) {
			World::unloadChunk(chunk);
		}
	}
	*/
}