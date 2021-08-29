#include "client.hpp"
#include "mesher.hpp"

#include "../server/server.hpp"
#include "../../graphics/types.hpp"

uint32_t		Client::radius;
uint32_t		Client::diameter;
uint32_t		Client::chunk_count;

ClientChunk** 	Client::client_chunks;
Player*			Client::player;

void Client::init()
{
	client_chunks = nullptr;
	player = new Player();
}

void Client::destroy()
{
	if (client_chunks != nullptr) {
		free(client_chunks);
	}
}

void Client::connect()
{
	int player_id = Server::addPlayer();
}

void Client::setViewDistance(uint32_t distance)
{
	if (client_chunks != nullptr) {
		// not the best way as it causes all the meshes to be regenerated
		for (int i = 0; i < chunk_count; i++)
		{
			client_chunks[i]->destroy();
			ClientChunk::free_chunk(client_chunks[i]);
		}
		free(client_chunks);
	}
	radius = distance;
	diameter = radius * 2 + 1;
	chunk_count = diameter * diameter * diameter;
	client_chunks = (ClientChunk**)malloc(chunk_count * sizeof(void*));
	memset(client_chunks, 0, chunk_count * sizeof(void*));
}

void Client::sync()
{
	Server::players->x = player->x;
	Server::players->y = player->y;
	Server::players->z = player->z;
}

void Client::update()
{
	int pcx = World::toChunkScale(player->x);
	int pcy = World::toChunkScale(player->y);
	int pcz = World::toChunkScale(player->z);

	int bcx = pcx - radius;
	int bcy = pcy - radius;
	int bcz = pcz - radius;

	// no point in updating if nothing changed
	if (client_chunks[0] != nullptr && pcx - client_chunks[0]->chunk->x == radius && pcy - client_chunks[0]->chunk->y == radius && pcz - client_chunks[0]->chunk->z == radius) {
		return;
	}

	ClientChunk** old_chunks = (ClientChunk**)malloc(chunk_count * sizeof(void*));
	memcpy(old_chunks, client_chunks, chunk_count * sizeof(void*));
	memset(client_chunks, 0, chunk_count * sizeof(void*));

#ifdef DEBUG
	uint64_t vertices = 0;
#endif

	for (int i = 0; i < chunk_count; i++)
	{
		if (old_chunks[i] == nullptr) {
			continue;
		}
		int cx = old_chunks[i]->chunk->x;
		int cy = old_chunks[i]->chunk->y;
		int cz = old_chunks[i]->chunk->z;

		if (abs(pcx - cx) > radius || abs(pcy - cy) > radius || abs(pcz - cz) > radius) {
			old_chunks[i]->destroy();
			ClientChunk::free_chunk(old_chunks[i]);
		} else {
			int index = ((cz - bcz) * diameter + (cy - bcy)) * diameter + (cx - bcx);
			client_chunks[index] = old_chunks[i];
		}
	}

	free(old_chunks);

	for (int z = 0; z < diameter; z++)
	{
		int iz = z * diameter;
		for (int y = 0; y < diameter; y++)
		{
			int izy = (iz + y) * diameter;
			for (int x = 0; x < diameter; x++)
			{
				int index = izy + x;

				if (client_chunks[index] == nullptr) {

					client_chunks[index] = ClientChunk::alloc_chunk();
					client_chunks[index]->init();

					client_chunks[index]->chunk = Client::getChunk(x + bcx, y + bcy, z + bcz);

					Mesher::generateMesh(client_chunks[index]);
				}
			#ifdef DEBUG
				vertices += client_chunks[index]->indice_count * 4 / 6;
			#endif
			}
		}
	}

#ifdef DEBUG
	{
		uint64_t chunks_available 	= ClientChunk::pool->poolCount();
		uint64_t chunks_used		= ClientChunk::pool->inCirculation();
		uint64_t chunks_total		= ClientChunk::pool->totalCount();
		uint64_t chunks_memory		= chunks_total * sizeof(ClientChunk) / (1024);
		uint64_t chunks_vmem		= vertices * sizeof(VertexPNT) / (1024 * 1024);
		printf("\n===========================\n"\
				 "=> UPDATED CLIENT CHUNKS <=\n"\
				 "===========================\n");
		printf("STATS: Client Chunks\n\t=> %ju client chunks in pool\n\t=> %ju client chunks in use\n\t=> %ju client chunks total using\n\t\t%ju KB of RAM\n\t\t%ju MB of VRAM for vertices\n",
				chunks_available, chunks_used, chunks_total, chunks_memory, chunks_vmem);
	}
#endif
}

Chunk* Client::getChunk(int32_t chunk_x, int32_t chunk_y, int32_t chunk_z)
{
	Chunk* chunk = World::getChunk(chunk_x, chunk_y, chunk_z);
	if (chunk == nullptr) {
		return Server::requestChunk(0, chunk_x, chunk_y, chunk_z);
	}
	return chunk;
}