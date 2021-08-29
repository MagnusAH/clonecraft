#pragma once

#include "vector4f.hpp"

struct alignas(32) Vector3f
{
	union {
		float vec[3];
		struct {
			float x, y, z;
		};
		struct {
			float r, g, b;
		};
	};
	Vector3f();
	Vector3f(float* vals);
	Vector3f(float a, float b, float c);
	~Vector3f();

	// Vector3f operator*(const Matrix3f& matrix);
	Vector3f operator+(const Vector3f& vector);
	Vector3f operator-(const Vector3f& vector);
	Vector3f operator+(const Vector4f& vector);

	Vector3f operator*(const float f);

	void normalize();
	static Vector3f normalize(Vector3f vector);

	float dot(Vector3f vb);
	static float dot(Vector3f va, Vector3f vb);

	Vector3f cross(Vector3f vb);
	static Vector3f cross(Vector3f va, Vector3f vb);
};