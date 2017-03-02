/*
Copyright 2015 William A Adams

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#pragma once

#ifndef linearalgebra_h
#define linearalgebra_h

#include "dproc_config.h"

typedef struct _mat2 {
	real m11, m12;
	real m21, m22;
} mat2;

typedef struct _mat3 {
	real m11, m12, m13;
	real m21, m22, m23;
	real m31, m32, m33;
} mat3;


typedef struct _mat4 {
	real m11, m12, m13, m14;
	real m21, m22, m23, m24;
	real m31, m32, m33, m34;
	real m41, m42, m43, m44;
} mat4;


#ifdef __cplusplus
extern "C" {
#endif

// Basic geometric vector type routines
	DPROC_EXPORT void realn_assign(const int dim, real *c, const real *a);
	DPROC_EXPORT void realn_add(const int dim, real * c, const real * a, const real * b);
	DPROC_EXPORT void realn_sub(const int dim, real * c, const real * a, const real * b);
	DPROC_EXPORT void realn_mul_scalar(const int dim, real * c, const real * a, const real scalar);
	DPROC_EXPORT void realn_div_scalar(const int dim, real * c, const real * a, const real scalar);
	DPROC_EXPORT void realn_neg(const int dim, real * c, const real * a);
	DPROC_EXPORT void realn_mul_realn(const size_t dim, real *c, const real *a, const real *b);

// Linear Algebra
	DPROC_EXPORT real realn_dot(const int dim, const real * a, const real * b);
	DPROC_EXPORT real realn_mag(const int dim, const real * c);	// magnitude of vector
	DPROC_EXPORT void realn_normalize(const int dim, real * c, const real * a);

// Specific to 1x3
	DPROC_EXPORT void real3_set(real3 c, real x, real y, real z);


	DPROC_EXPORT void real3_cross(real3 c, const real3 a, const real3 b);
	DPROC_EXPORT real real3_distance(const real3 a, const real3 b);
	DPROC_EXPORT real real3_radians_between_units(const real3 a, const real3 b);
	DPROC_EXPORT real real3_radians_between(const real3 a, const real3 b);


// generic matrix routines
	DPROC_EXPORT void matn_transpose(const size_t dim, real **a);

// matrix 2X2
void mat2_mul_scalar(mat2 &c, const mat2 &a, const real scalar);
void mat2_div_scalar(mat2 &c, const mat2 &a, const real scalar);
void mat2_neg(mat2 &c, const mat2 &a);
real mat2_determinant(const mat2 &a);
int mat2_inverse(mat2 &c, const mat2 &a);
real mat2_trace(const mat2 &a);
void mat2_transpose(const mat2 &c, const mat2 &a);
void mat2_set_identity(mat2 &c);
void mat2_mul_mat2(mat2 &c, const mat2 &a, const mat2 &b);
void mat2_get_row_order(real *c);
void mat2_get_col_order(real *c);

// Matrix 3X3
void mat3_add(mat3 &c, const mat3 &a, const mat3 &b);
void mat3_sub(mat3 &c, const mat3 &a, const mat3 &b);
void mat3_mul_scalar(mat3 &c, const mat3 &a, const real scalar);
void mat3_div_scalar(mat3 &c, const mat3 &a, const real scalar);
real mat3_determinant(const mat3 &a);
real mat3_trace(const mat3 &a);
void mat3_transpose(mat3 &c, const mat3 &a);
void mat3_mul_mat3(mat3 &c, const mat3 &a, const mat3 &b);
void row3_mul_mat3(real3 c, const real3 a, const mat3 &m);

void mat3_set_identity(mat3 &c);
void mat3_set_columns(mat3 &rotation, const real3 col1, const real3 col2, const real3 col3);

// Matrix 4x4
// c = transpose(a)
DPROC_EXPORT void mat4_transpose(mat4 &c, const mat4 &a);

// identity, also Kronecker delta
DPROC_EXPORT void mat4_set_identity(mat4 &c);

DPROC_EXPORT void mat4_mul_scalar(mat4 &c, const mat4 &a, const real scalar);
DPROC_EXPORT void mat4_div_scalar(mat4 &c, const mat4 &a, const real scalar);
DPROC_EXPORT real mat4_determinant(const mat4 &m);
DPROC_EXPORT void mat4_adjoint(mat4 &c, const mat4 &a);
DPROC_EXPORT int mat4_inverse(mat4 &c, const mat4 &a);
DPROC_EXPORT void mat4_mul_mat4(mat4 &c, const mat4 &a, const mat4 &b);

// assuming row vectors, multiply with matrix on the right
// c = a * m;
DPROC_EXPORT void row4_mul_mat4(real4 c, const real *a, const mat4 &m);

// assuming a column vector, multiply with matrix on left
// c = m * a;
DPROC_EXPORT void mat4_mul_col4(real4 c, const mat4 &m, const real *a);



#ifdef __cplusplus
}
#endif


// Simple arithmetic
#define real3_add(c, a, b) realn_add(3, c, a, b)
#define real3_sub(c, a, b) realn_sub(3, c, a, b)
#define real3_mul_scalar(c, a, scalar) realn_mul_scalar(3, c, a, scalar)
#define real3_mul_real3(c, a, b) realn_mul_realn(3, c, a, b)
#define real3_div_scalar(c, a, scalar) realn_div_scalar(3, c, a, scalar)
#define real3_neg(c, a) realn_neg(3, c, a)

#define  real3_dot(a, b) realn_dot(3, a, b)
#define  real3_mag(c) realn_mag(3, c)
#define  real3_normalize(c, a) realn_normalize(3, c, a)


#endif
