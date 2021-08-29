#pragma once

struct alignas(32) Vector2f
{
	union {
		float vec[3];
		struct {
			float x, y;
		};
		struct {
			float s, t;
		};
	};
	Vector2f();
	Vector2f(float* vals);
	Vector2f(float a, float b);
	~Vector2f();

	// Vector2f operator*(const Matrix2f& matrix);
};