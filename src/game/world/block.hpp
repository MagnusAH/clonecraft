#pragma once

struct Block
{
	int16_t id;
	int16_t meta;
};

/*
sign bit used for transparency flag
*/

enum BlockTypes: int16_t
			{
// null block, not part of a mesh
			AIR = 0,

// transparent blocks, part of transparent mesh
			GLASS,

// opaque blocks, part of opaque mesh
			DIRT = -32768,
			GRASS,
			STONE,
			WOOD
			};