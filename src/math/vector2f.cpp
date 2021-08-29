#include "vector2f.hpp"

#include <cstring>
#include <stdio.h>

Vector2f::Vector2f()
{

}

Vector2f::Vector2f(float* vals)
{
	memcpy(vec, vals, 2*sizeof(float));
}

Vector2f::Vector2f(float a, float b)
{
	vec[0] = a;
	vec[1] = b;
}

Vector2f::~Vector2f()
{

}