#pragma once

#include "../world/world.hpp"

struct ServerPlayer
{
	int id;
	float x, y, z;
	float lx, ly, lz;

	inline ServerPlayer()
	{
		id = 0;
		x = 0;
		y = 0;
		z = 0;
		lx = 0;
		ly = 0;
		lz = 0;
	}

	inline void update()
	{
		lx = x;
		ly = y;
		lz = z;
	}
};