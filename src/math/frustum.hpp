#pragma once

#include "vector3f.hpp"
#include "vector4f.hpp"
#include "matrix4f.hpp"
#include <cmath>

struct Plane
{
	Vector3f normal;
	float d;

	inline void fromPoints(Vector3f a, Vector3f b, Vector3f c)
	{
		Vector3f u = b-a;
		Vector3f v = c-a;

		normal = Vector3f::cross(u, v);
		normal.normalize();

		d = Vector3f::dot(Vector3f(-normal.x, -normal.y, -normal.z), a);
	}

	inline void from4(float a, float b, float c, float _d)
	{
		float scale = 1 / sqrt(a * a + b * b + c * c);
		normal = Vector3f(a * scale, b * scale, c * scale);
		d = _d * scale;
	}

	inline float distance(Vector3f p)
	{
		return Vector3f::dot(normal, p) + d;
	}
};

#define PLANE_TOP 		0
#define PLANE_BOTTOM	1
#define PLANE_LEFT		2
#define PLANE_RIGHT		3
#define PLANE_NEAR		4
#define PLANE_FAR		5

#include "../graphics/renderer.hpp"


struct Frustum
{
	Plane planes[6];

	inline bool inside(Vector3f point, float radius)
	{
		for (int i = 0; i < 6; i++)
		{
			if (planes[i].distance(point) + radius < 0) {
				return false;
			}
		}
		return true;
	}

	inline void setCamera(Player* player)
	{
		// real shit

		Matrix4f pvMatrix = player->getViewMatrix() * Renderer::projectionMatrix;
		float* mat = pvMatrix.mat;

		planes[PLANE_LEFT].from4(mat[3] + mat[0], mat[7] + mat[4], mat[11] + mat[8], mat[15] + mat[12]);
		planes[PLANE_RIGHT].from4(mat[3] - mat[0], mat[7] - mat[4], mat[11] - mat[8], mat[15] - mat[12]);

		planes[PLANE_TOP].from4(mat[3] - mat[1], mat[7] - mat[5], mat[11] - mat[9], mat[15] - mat[13]);
		planes[PLANE_BOTTOM].from4(mat[3] + mat[1], mat[7] + mat[5], mat[11] + mat[9], mat[15] + mat[13]);

		planes[PLANE_NEAR].from4(mat[3] + mat[2], mat[7] + mat[6], mat[11] + mat[10], mat[15] + mat[14]);
		planes[PLANE_FAR].from4(mat[3] - mat[2], mat[7] - mat[6], mat[11] - mat[10], mat[15] - mat[14]);
	}

};