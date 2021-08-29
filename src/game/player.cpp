#include "player.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

#include "../window/input.hpp"
#include "world/world.hpp"


Player::Player()
{
	x = 0;
	y = 0;
	z = 0;
	pitch = 0;
	yaw = 0;
	moveSpeed = 0.5f;
	rotSpeed = 0.0025f;
}

void Player::update()
{
	float s = sin(yaw);
	float c = cos(yaw);

	if (Input::key(GLFW_KEY_W) & INPUT_DOWN) {
		z -= moveSpeed * c;
		x -= moveSpeed * s;
	} else if (Input::key(GLFW_KEY_S) & INPUT_DOWN) {
		z += moveSpeed * c;
		x += moveSpeed * s;
	}
	if (Input::key(GLFW_KEY_A) & INPUT_DOWN) {
		x -= moveSpeed * c;
		z += moveSpeed * s;
	} else if (Input::key(GLFW_KEY_D) & INPUT_DOWN) {
		x += moveSpeed * c;
		z -= moveSpeed * s;
	}
	if (Input::key(GLFW_KEY_SPACE) & INPUT_DOWN) {
		y += moveSpeed;
	} else if (Input::key(GLFW_KEY_LEFT_SHIFT) & INPUT_DOWN) {
		y -= moveSpeed;
	}

	yaw -= Input::m_dx * rotSpeed;
	pitch -= Input::m_dy * rotSpeed;
}

Matrix4f Player::getViewMatrix()
{
	Matrix4f viewMatrix;
	viewMatrix.translate(-x, -y, -z);
	viewMatrix.rotateY(-yaw);
	viewMatrix.rotateX(-pitch);
	return viewMatrix;
}

int32_t Player::getCX()
{
	return World::toChunkScale(x);
}

int32_t Player::getCY()
{
	return World::toChunkScale(y);
}

int32_t Player::getCZ()
{
	return World::toChunkScale(z);
}