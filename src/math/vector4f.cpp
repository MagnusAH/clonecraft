#include "vector4f.hpp"

#include <immintrin.h>
#include <cstring>
#include <cmath>
#include <stdio.h>

Vector4f::Vector4f()
{

}

Vector4f::Vector4f(float* vals)
{
	memcpy(vec, vals, 4*sizeof(float));
}

Vector4f::Vector4f(float a, float b, float c, float d)
{
	vec[0] = a;
	vec[1] = b;
	vec[2] = c;
	vec[3] = d;
}

Vector4f::~Vector4f()
{

}

Vector4f Vector4f::normalize()
{
	float scale = 1 / sqrt(x * x + y * y + z * z + w * w);
	return Vector4f(x * scale, y * scale, z * scale, w * scale);
}

inline void printF4(float* f)
{
	printf("[%f, %f, %f, %f]\n", f[0], f[1], f[2], f[3]);
}

inline void printF8(float* f)
{
	printf("[%f, %f, %f, %f, %f, %f, %f, %f]\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
}

static const int mulm_idx[] = {0, 4, 2, 6, 1, 5, 3, 7};

Vector4f Vector4f::operator*(const Matrix4f& matrix)
{
	// transpose matrix
	__m256
		lda = _mm256_load_ps(matrix.mat),
		ldb = _mm256_load_ps(&matrix.mat[8]);

	__m256i
		midx = _mm256_load_si256((__m256i*)mulm_idx);

	__m256
		tpa = _mm256_permutevar8x32_ps(_mm256_shuffle_ps(lda, ldb, 0b01000100), midx),
		tpb = _mm256_permutevar8x32_ps(_mm256_shuffle_ps(lda, ldb, 0b11101110), midx);

	// load vector

	__m128
		ldv = _mm_load_ps(vec);
	__m256
		v2x = _mm256_insertf128_ps(_mm256_castps128_ps256(ldv), ldv, 1);

	// multiply
	__m256
		m0 = _mm256_mul_ps(tpa, v2x),
		m1 = _mm256_mul_ps(tpb, v2x);

	Vector4f out;

	_mm_store_ps(out.vec, _mm_hadd_ps(
		_mm_hadd_ps(_mm256_castps256_ps128(m0), *((__m128*)(&((float*)(&m0))[4]))),
		_mm_hadd_ps(_mm256_castps256_ps128(m1), *((__m128*)(&((float*)(&m1))[4])))
	));

	return out;

	// __m256
	// 	ha = _mm256_hadd_ps(m0, m1);

	// __m128
	// 	hal = _mm256_castps256_ps128(ha),
	// 	hah = *((__m128*)(&((float*)(&ha))[4]));

	// _mm256_store_ps(f0, ha);
	// printF4(f0);
	// printF4(&f0[4]);
	// printf("--------\n");

	return out;

	// Vector4f out;

	// out.vec[0] = vec[0]*matrix.mat[0] + vec[1]*matrix.mat[4] + vec[2]*matrix.mat[8] + vec[3]*matrix.mat[12];
	// out.vec[1] = vec[0]*matrix.mat[1] + vec[1]*matrix.mat[5] + vec[2]*matrix.mat[9] + vec[3]*matrix.mat[13];
	// out.vec[2] = vec[0]*matrix.mat[2] + vec[1]*matrix.mat[6] + vec[2]*matrix.mat[10] + vec[3]*matrix.mat[14];
	// out.vec[3] = vec[0]*matrix.mat[3] + vec[1]*matrix.mat[7] + vec[2]*matrix.mat[11] + vec[3]*matrix.mat[15];

	// return out;
}