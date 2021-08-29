#pragma once

#include "matrix4f.hpp"

struct alignas(32) Vector4f
{
	union {
		float vec[4];
		struct {
			float x, y, z, w;
		};
		struct {
			float r, g, b, a;
		};
	};
	Vector4f();
	Vector4f(float* vals);
	Vector4f(float a, float b, float c, float d);
	~Vector4f();

	Vector4f operator*(const Matrix4f& matrix);

	Vector4f normalize();
};