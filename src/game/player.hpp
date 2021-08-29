#pragma once

#include <stdint.h>

#include "../math/matrix4f.hpp"

struct Player
{
	float x, y, z;
	float pitch, yaw;
	float moveSpeed, rotSpeed;

	Player();

	void update();
	Matrix4f getViewMatrix();

	int32_t getCX();
	int32_t getCY();
	int32_t getCZ();
};