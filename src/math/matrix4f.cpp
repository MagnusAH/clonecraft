#include "matrix4f.hpp"

#include <immintrin.h>

#include <math.h>
#include <stdint.h>
#include <string.h>

#include <stdio.h>

#include "vector3f.hpp"

static const float IDENTITY[16] = {1, 0, 0, 0,
								   0, 1, 0, 0,
								   0, 0, 1, 0,
								   0, 0, 0, 1};

#define _mm256_reverse_ps(x) _mm256_permute_ps(_mm256_permute2f128_ps((x), (x), 1), 0b00011011)

Matrix4f::Matrix4f()
{
	memcpy(mat, IDENTITY, 16*sizeof(float));
}

Matrix4f::Matrix4f(float* vals)
{
	memcpy(mat, vals, 16*sizeof(float));
}

Matrix4f::~Matrix4f()
{

}

// TODO reorganize to use _mm256_load_ps, _mm256_set_ps kills performance, remove for loop
Matrix4f Matrix4f::operator*(const Matrix4f& mb)
{
	Matrix4f out;

	__m256 vb1 = _mm256_set_ps(mb.mat[0], mb.mat[4], mb.mat[8], mb.mat[12],
						  	   mb.mat[1], mb.mat[5], mb.mat[9], mb.mat[13]);
	__m256 vb2 = _mm256_set_ps(mb.mat[2], mb.mat[6], mb.mat[10], mb.mat[14],
						  	   mb.mat[3], mb.mat[7], mb.mat[11], mb.mat[15]);

	for (uint32_t i = 0; i < 4; i++)
	{
		uint32_t aidx = 4*i;
		__m256 va = _mm256_set_ps(mat[aidx], mat[aidx+1], mat[aidx+2], mat[aidx+3],
								  mat[aidx], mat[aidx+1], mat[aidx+2], mat[aidx+3]);
		// __m256 m1 = _mm256_mul_ps(va, vb1);
		// __m256 m2 = _mm256_mul_ps(va, vb2);
		// __m256 a1 = _mm256_hadd_ps(m1, m2);
		// __m256 a2 = _mm256_hadd_ps(a1, a1);
		va = _mm256_hadd_ps(_mm256_mul_ps(va, vb1), _mm256_mul_ps(va, vb2));
		va = _mm256_hadd_ps(va, va);
		// vectors are backwards and hadd interweaves the results
		out.mat[aidx] = ((float*)&va)[6];
		out.mat[aidx+1] = ((float*)&va)[2];
		out.mat[aidx+2] = ((float*)&va)[7];
		out.mat[aidx+3] = ((float*)&va)[3];
	}

	return out;
}

Matrix4f Matrix4f::operator/(const Matrix4f& mb)
{
	return (*this)*Matrix4f::inv(mb);
}

// temp function
inline float m_det(float a, float b, float c,
				 float i, float j, float k,
				 float x, float y, float z)
{
	// a *|j k, y z| - b * |i k, x z| + c * |i j, x y|
	return a * (j*z-k*y) - b * (i*z-k*x) + c * (i*y-j*x);
}

// TODO vectorize into 4 parallels, __m128 x 9
float Matrix4f::det(const Matrix4f mx)
{
	return mx.mat[0] * m_det(mx.mat[5], mx.mat[6], mx.mat[7], mx.mat[9], mx.mat[10], mx.mat[11], mx.mat[13], mx.mat[14], mx.mat[15])
		 - mx.mat[1] * m_det(mx.mat[4], mx.mat[6], mx.mat[7], mx.mat[8], mx.mat[10], mx.mat[11], mx.mat[12], mx.mat[14], mx.mat[15])
		 + mx.mat[2] * m_det(mx.mat[4], mx.mat[5], mx.mat[7], mx.mat[8], mx.mat[9], mx.mat[11], mx.mat[12], mx.mat[13], mx.mat[15])
		 - mx.mat[3] * m_det(mx.mat[4], mx.mat[5], mx.mat[6], mx.mat[8], mx.mat[9], mx.mat[10], mx.mat[12], mx.mat[13], mx.mat[14]);
}

static const int trans_idx[] = {0, 4, 2, 6, 1, 5, 3, 7};

Matrix4f Matrix4f::trans(const Matrix4f mx)
{
	__m256
		lda = _mm256_load_ps(mx.mat),
		ldb = _mm256_load_ps(&mx.mat[8]);
	__m256i
		idx = _mm256_load_si256((__m256i*)trans_idx);

	Matrix4f out;

	_mm256_store_ps(out.mat, _mm256_permutevar8x32_ps(_mm256_shuffle_ps(lda, ldb, 0b01000100), idx));
	_mm256_store_ps(&out.mat[8], _mm256_permutevar8x32_ps(_mm256_shuffle_ps(lda, ldb, 0b11101110), idx));

	return out;

	// Matrix4f out;
	// out.mat[0] = mx.mat[0];
	// out.mat[1] = mx.mat[4];
	// out.mat[2] = mx.mat[8];
	// out.mat[3] = mx.mat[12];
	// out.mat[4] = mx.mat[1];
	// out.mat[5] = mx.mat[5];
	// out.mat[6] = mx.mat[9];
	// out.mat[7] = mx.mat[13];
	// out.mat[8] = mx.mat[2];
	// out.mat[9] = mx.mat[6];
	// out.mat[10] = mx.mat[10];
	// out.mat[11] = mx.mat[14];
	// out.mat[12] = mx.mat[3];
	// out.mat[13] = mx.mat[7];
	// out.mat[14] = mx.mat[11];
	// out.mat[15] = mx.mat[15];
	// return out;
}

inline void printF8(float* f)
{
	printf("[%f, %f, %f, %f, %f, %f, %f, %f]\n", f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
}

inline void printF4(float* f)
{
	printf("[%f, %f, %f, %f]\n", f[0], f[1], f[2], f[3]);
}

static const int inv_idx0[8] = {0, 4, 2, 6,
							   1, 5, 3, 7};
static const int inv_idx1[8] = {2, 6, 0, 4,
								3, 7, 2, 6};

static const float inv_alt[4] = {1, -1, 1, -1};

static const float inv_neg1 = -1.0f;

// faster than glm (take 93% of the time glm does on my computer)
Matrix4f Matrix4f::inv(const Matrix4f mx)
{
	// data transfer
	__m256
		lda = _mm256_reverse_ps(_mm256_load_ps(mx.mat)),
		ldb = _mm256_reverse_ps(_mm256_load_ps(&(mx.mat[8])));

	__m256
		pma0 = _mm256_permute_ps(lda, 0b01000000), pma8 = _mm256_permute_ps(ldb, 0b01000000),
		pmb0 = _mm256_permute_ps(lda, 0b10100101), pmb8 = _mm256_permute_ps(ldb, 0b10100101),
		pmc0 = _mm256_permute_ps(lda, 0b11111110), pmc8 = _mm256_permute_ps(ldb, 0b11111110);

	__m128
		exa0 = _mm256_castps256_ps128(pma0), exa4 = *((__m128*)(&((float*)(&pma0))[4])), exa8 = _mm256_castps256_ps128(pma8), exa12 = *((__m128*)(&((float*)(&pma8))[4])),
		exb0 = _mm256_castps256_ps128(pmb0), exb4 = *((__m128*)(&((float*)(&pmb0))[4])), exb8 = _mm256_castps256_ps128(pmb8), exb12 = *((__m128*)(&((float*)(&pmb8))[4])),
		exc0 = _mm256_castps256_ps128(pmc0), exc4 = *((__m128*)(&((float*)(&pmc0))[4])), exc8 = _mm256_castps256_ps128(pmc8), exc12 = *((__m128*)(&((float*)(&pmc8))[4]));

	// __m128 can be faster than __m256 when it leads to data reuse
	__m128
		ia01 = _mm_fmsub_ps(exb8, exc12, _mm_mul_ps(exc8, exb12)),
		ib01 = _mm_fmsub_ps(exc8, exa12, _mm_mul_ps(exa8, exc12)),
		ic01 = _mm_fmsub_ps(exa8, exb12, _mm_mul_ps(exb8, exa12));

	__m128
		ia23 = _mm_fmsub_ps(exc4, exb0, _mm_mul_ps(exb4, exc0)),
		ib23 = _mm_fmsub_ps(exa4, exc0, _mm_mul_ps(exc4, exa0)),
		ic23 = _mm_fmsub_ps(exb4, exa0, _mm_mul_ps(exa4, exb0));

	// calculate cofactor without multiplying by alternating sign grid
	__m128
		r0 = _mm_fmadd_ps(exa0, ia01, _mm_fmadd_ps(exb0, ib01, _mm_mul_ps(exc0, ic01))),
		r1 = _mm_fmadd_ps(exa4, ia01, _mm_fmadd_ps(exb4, ib01, _mm_mul_ps(exc4, ic01))),
		r2 = _mm_fmadd_ps(exa8, ia23, _mm_fmadd_ps(exb8, ib23, _mm_mul_ps(exc8, ic23))),
		r3 = _mm_fmadd_ps(exa12, ia23, _mm_fmadd_ps(exb12, ib23, _mm_mul_ps(exc12, ic23)));

	// calculate determinant
	float dets[4];
	_mm_store_ps(dets, _mm_mul_ps(r0, _mm256_castps256_ps128(_mm256_reverse_ps(lda))));
	float det = (dets[0]-dets[1]+dets[2]-dets[3]);

	// fuse inverse determinant and alternating sign grid
	__m128
		grid = _mm_div_ps(_mm_load_ps(inv_alt), _mm_broadcast_ss(&det));

	// multiply by alternating sign grid and inverse determinant
	r0 = _mm_mul_ps(r0, grid);
	r2 = _mm_mul_ps(r2, grid);
	grid = _mm_mul_ps(grid, _mm_broadcast_ss(&inv_neg1));
	r1 = _mm_mul_ps(r1, grid);
	r3 = _mm_mul_ps(r3, grid);

	// transpose and store
	__m128
		tra01 = _mm_permute_ps(_mm_shuffle_ps(r0, r1, 0b01000100), 0b11011000),
		trb01 = _mm_permute_ps(_mm_shuffle_ps(r0, r1, 0b11101110), 0b11011000),
		tra23 = _mm_permute_ps(_mm_shuffle_ps(r2, r3, 0b01000100), 0b11011000),
		trb23 = _mm_permute_ps(_mm_shuffle_ps(r2, r3, 0b11101110), 0b11011000);

	Matrix4f out;

	_mm_store_ps(out.mat, _mm_shuffle_ps(tra01, tra23, 0b01000100));
	_mm_store_ps(&out.mat[4], _mm_shuffle_ps(tra01, tra23, 0b11101110));
	_mm_store_ps(&out.mat[8], _mm_shuffle_ps(trb01, trb23, 0b01000100));
	_mm_store_ps(&out.mat[12], _mm_shuffle_ps(trb01, trb23, 0b11101110));

	return out;

/* vector 1.0
	// constructing operands
	__m256
		a0 = pma0,															a1 = _mm256_insertf128_ps(pma0, exa0, 1),
		b0 = pmb0,															b1 = _mm256_insertf128_ps(pmb0, exb0, 1),
		c0 = pmc0,															c1 = _mm256_insertf128_ps(pmc0, exc0, 1);
	__m256
		i0 = _mm256_insertf128_ps(pma8, exa8, 1),							i1 = _mm256_insertf128_ps(_mm256_castps128_ps256(exa4), exa4, 1),
		j0 = _mm256_insertf128_ps(pmb8, exb8, 1),							j1 = _mm256_insertf128_ps(_mm256_castps128_ps256(exb4), exb4, 1),
		k0 = _mm256_insertf128_ps(pmc8, exc8, 1),							k1 = _mm256_insertf128_ps(_mm256_castps128_ps256(exc4), exc4, 1);
	__m256
		x0 = _mm256_insertf128_ps(_mm256_castps128_ps256(exa12), exa12, 1), x1 = pma8,
		y0 = _mm256_insertf128_ps(_mm256_castps128_ps256(exb12), exb12, 1), y1 = pmb8,
		z0 = _mm256_insertf128_ps(_mm256_castps128_ps256(exc12), exc12, 1), z1 = pmc8;

	// cofactor without the sign grid
	__m256
		f0 = _mm256_sub_ps(_mm256_add_ps(
				_mm256_mul_ps(a0, _mm256_fmsub_ps(j0, z0, _mm256_mul_ps(k0, y0))),
				_mm256_mul_ps(c0, _mm256_fmsub_ps(i0, y0, _mm256_mul_ps(j0, x0)))
		),
				_mm256_mul_ps(b0, _mm256_fmsub_ps(i0, z0, _mm256_mul_ps(k0, x0)))
		);
	__m256
		f1 = _mm256_sub_ps(_mm256_add_ps(
				_mm256_mul_ps(a1, _mm256_fmsub_ps(j1, z1, _mm256_mul_ps(k1, y1))),
				_mm256_mul_ps(c1, _mm256_fmsub_ps(i1, y1, _mm256_mul_ps(j1, x1)))
		),
				_mm256_mul_ps(b1, _mm256_fmsub_ps(i1, z1, _mm256_mul_ps(k1, x1)))
		);

	// calcualte determinant
	float dets[4];
	_mm_store_ps(dets, _mm_mul_ps(_mm256_castps256_ps128(f0), _mm256_castps256_ps128(_mm256_reverse_ps(lda))));
	float detp = 1/(dets[0]-dets[1]+dets[2]-dets[3]);
	float detn = -1*detp;

	// multiply cofactor by inverse determinant and sign grid
	__m128
		dp = _mm_broadcast_ss(&detp),
		dn = _mm_broadcast_ss(&detn);
	__m256
		grid = _mm256_permute_ps(_mm256_insertf128_ps(_mm256_castps128_ps256(_mm_shuffle_ps(dn, dp, 0b00000000)), _mm_shuffle_ps(dp, dn, 0b00000000), 1), 0b01110010); //01110010

	f0 = _mm256_mul_ps(f0, grid);
	f1 = _mm256_mul_ps(f1, grid);

	// transpose
	__m256i
		idx0 = _mm256_load_si256((__m256i*)inv_idx0),
		idx1 = _mm256_load_si256((__m256i*)inv_idx1);

	__m256
		t0 = _mm256_permute_ps(_mm256_shuffle_ps(_mm256_permutevar8x32_ps(f0, idx0), _mm256_permutevar8x32_ps(f1, idx0), 0b01000100), 0b11100100),
		t1 = _mm256_permute_ps(_mm256_shuffle_ps(_mm256_permutevar8x32_ps(f0, idx1), _mm256_permutevar8x32_ps(f1, idx1), 0b01000100), 0b11100100);

	Matrix4f out;

	_mm256_store_ps(out.mat, t0);
	_mm256_store_ps(&out.mat[8], t1);

	return out;

	/**/

/*
	// SCALAR

	Matrix4f out;
	float pdt = 1.0f/Matrix4f::det(mx);
	float ndt = pdt*-1;

	// (0, [0, 1, 2, 3])
	out.mat[0] = m_det(mx.mat[5], mx.mat[6], mx.mat[7],
					 mx.mat[9], mx.mat[10], mx.mat[11],
					 mx.mat[13], mx.mat[14], mx.mat[15])*pdt;
	out.mat[4] = m_det(mx.mat[4], mx.mat[6], mx.mat[7],
					 mx.mat[8], mx.mat[10], mx.mat[11],
					 mx.mat[12], mx.mat[14], mx.mat[15])*ndt;
	out.mat[8] = m_det(mx.mat[4], mx.mat[5], mx.mat[7],
					 mx.mat[8], mx.mat[9], mx.mat[11],
					 mx.mat[12], mx.mat[13], mx.mat[15])*pdt;
	out.mat[12] = m_det(mx.mat[4], mx.mat[5], mx.mat[6],
					 mx.mat[8], mx.mat[9], mx.mat[10],
					 mx.mat[12], mx.mat[13], mx.mat[14])*ndt;
	// (1, [0, 1, 2, 3])
	out.mat[1] = m_det(mx.mat[1], mx.mat[2], mx.mat[3],
					 mx.mat[9], mx.mat[10], mx.mat[11],
					 mx.mat[13], mx.mat[14], mx.mat[15])*ndt;
	out.mat[5] = m_det(mx.mat[0], mx.mat[2], mx.mat[3],
					 mx.mat[8], mx.mat[10], mx.mat[11],
					 mx.mat[12], mx.mat[14], mx.mat[15])*pdt;
	out.mat[9] = m_det(mx.mat[0], mx.mat[1], mx.mat[3],
					 mx.mat[8], mx.mat[9], mx.mat[11],
					 mx.mat[12], mx.mat[13], mx.mat[15])*ndt;
	out.mat[13] = m_det(mx.mat[0], mx.mat[1], mx.mat[2],
					 mx.mat[8], mx.mat[9], mx.mat[10],
					 mx.mat[12], mx.mat[13], mx.mat[14])*pdt;
	// (2, [0, 1, 2, 3])
	out.mat[2] = m_det(mx.mat[1], mx.mat[2], mx.mat[3],
					 mx.mat[5], mx.mat[6], mx.mat[7],
					 mx.mat[13], mx.mat[14], mx.mat[15])*pdt;
	out.mat[6] = m_det(mx.mat[0], mx.mat[2], mx.mat[3],
					 mx.mat[4], mx.mat[6], mx.mat[7],
					 mx.mat[12], mx.mat[14], mx.mat[15])*ndt;
	out.mat[10] = m_det(mx.mat[0], mx.mat[1], mx.mat[3],
					 mx.mat[4], mx.mat[5], mx.mat[7],
					 mx.mat[12], mx.mat[13], mx.mat[15])*pdt;
	out.mat[14] = m_det(mx.mat[0], mx.mat[1], mx.mat[2],
					 mx.mat[4], mx.mat[5], mx.mat[6],
					 mx.mat[12], mx.mat[13], mx.mat[14])*ndt;
	// (3, [0, 1, 2, 3])
	out.mat[3] = m_det(mx.mat[1], mx.mat[2], mx.mat[3],
					 mx.mat[5], mx.mat[6], mx.mat[7],
					 mx.mat[9], mx.mat[10], mx.mat[11])*ndt;
	out.mat[7] = m_det(mx.mat[0], mx.mat[2], mx.mat[3],
					 mx.mat[4], mx.mat[6], mx.mat[7],
					 mx.mat[8], mx.mat[10], mx.mat[11])*pdt;
	out.mat[11] = m_det(mx.mat[0], mx.mat[1], mx.mat[3],
					 mx.mat[4], mx.mat[5], mx.mat[7],
					 mx.mat[8], mx.mat[9], mx.mat[11])*ndt;
	out.mat[15] = m_det(mx.mat[0], mx.mat[1], mx.mat[2],
					 mx.mat[4], mx.mat[5], mx.mat[6],
					 mx.mat[8], mx.mat[9], mx.mat[10])*pdt;

	return out;
	/**/
}

void Matrix4f::identity()
{
	memcpy(mat, IDENTITY, 16*sizeof(float));
}

void Matrix4f::setProjection(float aspectRatio, float fov, float near, float far)
{
	memcpy(mat, IDENTITY, 16*sizeof(float));
	float scale = atan(fov * 0.5);
	mat[0] = 1 / (scale * aspectRatio);
	mat[5] = 1 / (scale);
	mat[10] = -(far + near) / (far - near);
	mat[11] = -1;
	mat[14] = -(2 * far * near) / (far - near);
	mat[15] = 0;
}

void Matrix4f::translate(const float x, const float y, const float z)
{
	mat[12] += x;
	mat[13] += y;
	mat[14] += z;
}

void Matrix4f::setTransform(const float x, const float y, const float z)
{
	mat[12] = x;
	mat[13] = y;
	mat[14] = z;
}

void Matrix4f::rotateX(const float angle)
{
	float c = cos(angle);
	float s = sin(angle);
	Matrix4f rotation;
	rotation.mat[5] = c;
	rotation.mat[6] = s;
	rotation.mat[9] = -s;
	rotation.mat[10] = c;
	(*this) = (*this) * rotation;
	// (*this) = (*this) * Matrix4f::trans(rotation);
}

void Matrix4f::rotateY(const float angle)
{
	float c = cos(angle);
	float s = sin(angle);
	Matrix4f rotation;
	rotation.mat[0] = c;
	rotation.mat[2] = -s;
	rotation.mat[8] = s;
	rotation.mat[10] = c;
	(*this) = (*this) * rotation;
	// (*this) = (*this) * Matrix4f::trans(rotation);
}

void Matrix4f::rotateZ(const float angle)
{
	float c = cos(angle);
	float s = sin(angle);
	Matrix4f rotation;
	rotation.mat[0] = c;
	rotation.mat[1] = s;
	rotation.mat[4] = -s;
	rotation.mat[5] = c;
	(*this) = (*this) * rotation;
}