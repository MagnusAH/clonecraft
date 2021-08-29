#include "mesher.hpp"

#include "client.hpp"

#include "../../game/mesh.hpp"

void Mesher::generateMesh(ClientChunk* client_chunk)
{
	Block* blocks = client_chunk->chunk->block;

	Mesh mesh;

	int cx = client_chunk->chunk->x;
	int cy = client_chunk->chunk->y;
	int cz = client_chunk->chunk->z;

	Chunk* px = Client::getChunk(cx + 1, cy, cz);
	Chunk* nx = Client::getChunk(cx - 1, cy, cz);
	Chunk* py = Client::getChunk(cx, cy + 1, cz);
	Chunk* ny = Client::getChunk(cx, cy - 1, cz);
	Chunk* pz = Client::getChunk(cx, cy, cz + 1);
	Chunk* nz = Client::getChunk(cx, cy, cz - 1);

	for (int z = 0; z < CHUNK_SIZE; z++)
	{
		int iz = z * CHUNK_SIZE;
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			int izy = (iz + y) * CHUNK_SIZE;
			for (int x = 0; x < CHUNK_SIZE; x++)
			{
				int index = izy + x;
				if (blocks[index].id < 0) {
					float mx = (float)x - CHUNK_SIZE / 2;
					float my = (float)y - CHUNK_SIZE / 2;
					float mz = (float)z - CHUNK_SIZE / 2;
					// x check
					if (x + 1 == CHUNK_SIZE) {
						if (px->block[izy].id >= 0) {
							mesh.pushRight(mx, my, mz);
						}
					}
					else if (blocks[index + 1].id >= 0) {
						mesh.pushRight(mx, my, mz);
					}
					if (x == 0) {
						if (nx->block[izy + CHUNK_SIZE - 1].id >= 0) {
							mesh.pushLeft(mx, my, mz);
						}
					}
					else if (blocks[index - 1].id >= 0) {
						mesh.pushLeft(mx, my, mz);
					}

					// y check
					if (y + 1 == CHUNK_SIZE) {
						if (py->block[iz * CHUNK_SIZE + x].id >= 0) {
							mesh.pushTop(mx, my, mz);
						}
					}
					else if (blocks[index + CHUNK_SIZE].id >= 0) {
						mesh.pushTop(mx, my, mz);
					}
					if (y == 0) {
						if (ny->block[(iz + CHUNK_SIZE - 1) * CHUNK_SIZE + x].id >= 0) {
							mesh.pushBottom(mx, my, mz);
						}
					}
					else if (blocks[index - CHUNK_SIZE].id >= 0) {
						mesh.pushBottom(mx, my, mz);
					}

					// z check
					if (z + 1 == CHUNK_SIZE) {
						if (pz->block[y * CHUNK_SIZE + x].id >= 0) {
							mesh.pushFront(mx, my, mz);
						}
					}
					else if (blocks[index + CHUNK_SIZE * CHUNK_SIZE].id >= 0) {
						mesh.pushFront(mx, my, mz);
					}
					if (z == 0) {
						if (nz->block[((CHUNK_SIZE - 1) * CHUNK_SIZE + y) * CHUNK_SIZE + x].id >= 0) {
							mesh.pushBack(mx, my, mz);
						}
					}
					else if (blocks[index - CHUNK_SIZE * CHUNK_SIZE].id >= 0) {
						mesh.pushBack(mx, my, mz);
					}

				}
			}
		}
	}

	Model model;
	model.load(&mesh.vertices[0], mesh.vertices.size(), &mesh.indices[0], mesh.indices.size());

	if (! glIsBuffer(model.vertice_vbo)) {
		printf("FUCKED\n");
	}

	client_chunk->indice_count	= model.indice_count;
	// client_chunk->vbo_indices	= model.indice_vbo;
	client_chunk->vbo_vertices	= model.vertice_vbo;
}