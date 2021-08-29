#pragma once

#include <string.h>

#include "../math/vector3f.hpp"
#include "../math/vector2f.hpp"

union VertexPNT {
	struct {
		float x, y, z;
		float nx, ny, nz;
		float s, t;
	};
	float v[8];

	inline VertexPNT(Vector3f vertex, Vector3f normal, Vector2f texCoord)
	{
		memcpy(&x, &vertex, 3*sizeof(float));
		memcpy(&nx, &normal, 3*sizeof(float));
		memcpy(&s, &texCoord, 2*sizeof(float));
	}
};