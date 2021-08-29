#include "vector3f.hpp"

#include <cstring>
#include <stdio.h>
#include <math.h>

Vector3f::Vector3f()
{
	memset(this, 0, 3 * sizeof(float));
}

Vector3f::Vector3f(float* vals)
{
	memcpy(vec, vals, 3*sizeof(float));
}

Vector3f::Vector3f(float a, float b, float c)
{
	vec[0] = a;
	vec[1] = b;
	vec[2] = c;
}

Vector3f::~Vector3f()
{

}

Vector3f Vector3f::operator+(const Vector3f& vector)
{
	return Vector3f(x + vector.x, y + vector.y, z + vector.z);
}

Vector3f Vector3f::operator-(const Vector3f& vector)
{
	return Vector3f(x - vector.x, y - vector.y, z - vector.z);
}

Vector3f Vector3f::operator+(const Vector4f& vector)
{
	return Vector3f(x + vector.x, y + vector.y, z + vector.z);
}

Vector3f Vector3f::operator*(const float f)
{
	return Vector3f(x*f, y*f, z*f);
}

void Vector3f::normalize()
{
	float scale = 1 / sqrt(x*x + y*y + z*z);
	x *= scale;
	y *= scale;
	z *= scale;
}

Vector3f Vector3f::normalize(Vector3f v)
{
	float scale = 1 / sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	return Vector3f(v.x * scale, v.y * scale, v.z * scale);
}

float Vector3f::dot(Vector3f vb)
{
	return x*vb.x + y*vb.y + z*vb.z;
}

float Vector3f::dot(Vector3f va, Vector3f vb)
{
	return va.x*vb.x + va.y*vb.y + va.z*vb.z;
}

Vector3f Vector3f::cross(Vector3f vb)
{
	return Vector3f(y*vb.z - z*vb.y, z*vb.x - x*vb.z, x*vb.y - y*vb.x);
}

Vector3f Vector3f::cross(Vector3f va, Vector3f vb)
{
	return Vector3f(va.y*vb.z - va.z*vb.y, va.z*vb.x - va.x*vb.z, va.x*vb.y - va.y*vb.x);
}