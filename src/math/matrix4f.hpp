#pragma once

struct alignas(32) Matrix4f {
	float mat[16];

	Matrix4f();
	Matrix4f(float* vals);
	~Matrix4f();

	// x = MatB, y = MatA

	Matrix4f operator*(const Matrix4f& matrix);
	Matrix4f operator/(const Matrix4f& matrix);

	static float det(const Matrix4f matrix);
	static Matrix4f trans(const Matrix4f matrix);
	static Matrix4f inv(const Matrix4f matrix);

	void identity();
	void setProjection(float aspectRatio, float fov, float near, float far);
	void translate(const float x, const float y, const float z);
	void setTransform(const float x, const float y, const float z);
	void rotate(const float angle, float x, float y, float z);
	void rotateX(const float angle);
	void rotateY(const float angle);
	void rotateZ(const float angle);

	static Matrix4f getView(float x, float y, float z, float pitch, float yaw);
};