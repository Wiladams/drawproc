/*
Copyright 2015 William A Adams

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/



#include "linearalgebra.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

// general methods
// copy 
// c = a
void realn_assign(const int dim, real *c, const real *a)
{
	for (int idx = 0; idx < dim; idx++) {
		c[idx] = a[idx];
	}
}

void realn_add(const int dim, real * c, const real * a, const real *b)
{
	for (int i = 0; i < dim; i++)
	{
		c[i] = a[i] + b[i];
	}
}

void realn_sub(const int dim, real *c, const real *a, const real *b)
{
	for (int i = 0; i < dim; i++)
	{
		c[i] = a[i] - b[i];
	}
}



void realn_mul_realn(const size_t dim, real *c, const real *a, const real *b)
{
	for (size_t idx = 0; idx < dim; idx++) {
		c[idx] = a[idx] * b[idx];
	}
}

void realn_mul_scalar(const int dim, real *c, const real *a, const real scalar)
{
	for (int idx = 0; idx < dim; idx++){
		c[idx] = a[idx] * scalar;
	}
}

void realn_div_scalar(const int dim, real *c, const real *a, const real scalar)
{
	realn_mul_scalar(dim, c, a, 1 / scalar);
}

void realn_neg(const int dim, real *c, const real *a)
{
	for (int i = 0; i < dim; i++)
	{
		c[i] = -a[i];
	}
}

// Linear algebra
real realn_dot(const int dim, const real * a, const real * b)
{
	real c = 0;
	for (int i = 0; i < dim; i++)
	{
		c += a[i] * b[i];
	}

	return c;
}

real realn_mag(const int dim, const real * a)
{
	real lsquared = realn_dot(dim, a, a);
	return sqrt(lsquared);
}

void realn_normalize(const int dim, real * c, const real * a)
{
	real mag = realn_mag(dim, a);
	realn_div_scalar(dim, c, a, mag);
}





// Specific to real3
void real3_set(real3 c, real x, real y, real z)
{
	c[0] = x;
	c[1] = y;
	c[2] = z;
}



// c = a cross b
void real3_cross(real3 c, const real3 a, const real3 b)
{
	c[0] = (a[1] * b[2]) - (a[2] * b[1]);
	c[1] = (a[2] * b[0]) - (a[0] * b[2]);
	c[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

real real3_distance(const real3 a, const real3 b)
{
	real3 c;
	real3_sub(c, b, a);
	return real3_mag(c);
}

real real3_radians_between_units(const real3 a, const real3 b)
{
	return acos(real3_dot(a, b));
}

real real3_radians_between(const real3 a, const real3 b)
{
	real3 aunit;
	real3 bunit;

	real3_normalize(aunit, a);
	real3_normalize(bunit, b);
	
	return real3_radians_between_units(aunit, bunit);
}



/*
	Matrix routines
*/
// general matrix routines

// in-place transpose
void matn_transpose(const size_t dim, real **a)
{
	size_t i, j;
	real tmp;

	for (i = 1; i<dim; i++) {
		for (j = 0; j<i; j++) {
			tmp = a[i][j];
			a[i][j] = a[j][i];
			a[j][i] = tmp;
		}
	}
}


// 2 X 2 Matrix
void mat2_mul_scalar(mat2 &c, const mat2 &a, const real scalar)
{
	// row 1
	c.m11 = a.m11 * scalar;
	c.m12 = a.m12 * scalar;

	// row 2
	c.m21 = a.m21 * scalar;
	c.m22 = a.m22 * scalar;
}

void mat2_div_scalar(mat2 &c, const mat2 &a, const real scalar)
{
	real oneover = 1.0f / scalar;
	// row 1
	c.m11 = a.m11 * oneover;
	c.m12 = a.m12 * oneover;

	// row 2
	c.m21 = a.m21 * oneover;
	c.m22 = a.m22 * oneover;
}

void mat2_neg(mat2 &c, const mat2 &a)
{
	c.m11 = -a.m11;
	c.m12 = -a.m12;

	c.m21 = -a.m21;
	c.m22 = -a.m22;
}

real mat2_determinant(const mat2 &a)
{
	return (a.m11 * a.m22) - (a.m12 * a.m21);
}

// Calculate inverse of 2x2 matrix
// c = inverse(a)
int mat2_inverse(mat2 &c, const mat2 &a)
{
	real det = (a.m11 * a.m22) - (a.m12 * a.m21);
	if (det == 0) {
		// error when determinant == 0
		// cannot calculate inverse
		return -1;
	}
	
	// temp matrix used to do final calculation
	mat2 mattmp;
	mattmp.m11 = a.m22;
	mattmp.m12 = -a.m12;
	mattmp.m21 = -a.m21;
	mattmp.m22 = a.m11;
	mat2_div_scalar(c, mattmp, det);

	return 0;
}

// Calculate trace of 2x2 matrix
// trace is the sum of the diagonal elements
real mat2_trace(const mat2 &a)
{
	return a.m11 + a.m22;
}

// Create transpose of 2x2 matrix
// Assume c and a do NOT occupy the same data space
void mat2_transpose(mat2 &c, const mat2 &a)
{
	c.m11 = a.m11;
	c.m12 = a.m21;

	c.m21 = a.m12;
	c.m22 = a.m22;
}

void mat2_set_identity(mat2 &c)
{
	// memset to zero
	c.m12 = 0;
	c.m21 = 0;

	// set the ones we need to be one
	c.m11 = 1;
	c.m22 = 1;
}

// c = mat2 * mat2
void mat2_mul_mat2(mat2 &c, const mat2 &a, const mat2 &b)
{
	c.m11 = a.m11*b.m11 + a.m12*b.m21;
	c.m12 = a.m11*b.m12 + a.m12*b.m22;

	c.m21 = a.m21*b.m11 + a.m22*b.m21;
	c.m22 = a.m21*b.m12 + a.m22*b.m22;
}


// 3 X 3 Matrix

//
//	a  b  c
//  d  e  f
//  g  h  i
//


void mat3_neg(mat3 &c, const mat3 &a)
{
	// row 1
	c.m11 = -a.m11;
	c.m12 = -a.m12;
	c.m13 = -a.m13;

	// row 2
	c.m21 = -a.m21;
	c.m22 = -a.m22;
	c.m23 = -a.m23;

	// row 3
	c.m31 = -a.m31;
	c.m32 = -a.m32;
	c.m33 = -a.m33;
}

void mat3_add(mat3 &c, const mat3 &a, const mat3 &b)
{
	// row 1
	c.m11 = a.m11 + b.m11;
	c.m12 = a.m12 + b.m12;
	c.m13 = a.m13 + b.m13;

	// row 2
	c.m21 = a.m21 + b.m21;
	c.m22 = a.m22 + b.m22;
	c.m23 = a.m23 + b.m23;

	// row 3
	c.m31 = a.m31 + b.m31;
	c.m32 = a.m32 + b.m32;
	c.m33 = a.m33 + b.m33;
}

void mat3_sub(mat3 &c, const mat3 &a, const mat3 &b)
{
	// row 1
	c.m11 = a.m11 - b.m11;
	c.m12 = a.m12 - b.m12;
	c.m13 = a.m13 - b.m13;

	// row 2
	c.m21 = a.m21 - b.m21;
	c.m22 = a.m22 - b.m22;
	c.m23 = a.m23 - b.m23;

	// row 3
	c.m31 = a.m31 - b.m31;
	c.m32 = a.m32 - b.m32;
	c.m33 = a.m33 - b.m33;
}

void mat3_mul_scalar(mat3 &c, const mat3 &a, const real scalar)
{
	// row 1
	c.m11 = a.m11 * scalar;
	c.m12 = a.m12 * scalar;
	c.m13 = a.m13 * scalar;

	// row 2
	c.m21 = a.m21 * scalar;
	c.m22 = a.m22 * scalar;
	c.m23 = a.m23 * scalar;

	// row 3
	c.m31 = a.m31 * scalar;
	c.m32 = a.m32 * scalar;
	c.m33 = a.m33 * scalar;

}

void mat3_div_scalar(mat3 &c, const mat3 &a, const real scalar)
{
	real fac = 1 / scalar;
	mat3_mul_scalar(c, a, fac);
}

//
//	a  b  c
//  d  e  f
//  g  h  i
//
real mat3_determinant(const mat3 &a)
{
	// (aei +bfg + cdh) - (ceg + bdi + afh)
	real part1 = a.m11*a.m22*a.m33 + a.m12*a.m23*a.m31 + a.m13*a.m21*a.m32;
	real part2 = a.m13*a.m22*a.m31 + a.m12*a.m21*a.m33 + a.m11*a.m23*a.m32;

	return part1 - part2;
}

// a + e + i
real mat3_trace(const mat3 &a)
{
	return (a.m11 + a.m22 + a.m33);
}

// Assume c and a do NOT occupy the same data space
void mat3_transpose(mat3 &c, const mat3 &a)
{
	c.m11 = a.m11;
	c.m12 = a.m21;
	c.m13 = a.m31;

	c.m21 = a.m12;
	c.m22 = a.m22;
	c.m23 = a.m32;

	c.m31 = a.m13;
	c.m32 = a.m23;
	c.m33 = a.m33;
}

void mat3_set_identity(mat3 &c)
{
	memset(&c, 0, sizeof(mat3));

	c.m11 = 1;
	c.m22 = 1;
	c.m33 = 1;
}

void mat3_set_columns(mat3 &c, const real3 col1, const real3 col2, const real3 col3)
{
	c.m11 = col1[0];
	c.m21 = col1[1];
	c.m31 = col1[2];

	c.m12 = col2[0];
	c.m22 = col2[1];
	c.m32 = col2[2];

	c.m13 = col3[0];
	c.m23 = col3[1];
	c.m33 = col3[2];
}

// c = mat3 * mat3
void mat3_mul_mat3(mat3 &c, const mat3 &a, const mat3 &b)
{
	c.m11 = a.m11*b.m11 + a.m12*b.m21 + a.m13*b.m31;
	c.m12 = a.m11*b.m12 + a.m12*b.m22 + a.m13*b.m32;
	c.m13 = a.m11*b.m13 + a.m12*b.m23 + a.m13*b.m33;

	c.m21 = a.m21*b.m11 + a.m22*b.m21 + a.m23*b.m31;
	c.m22 = a.m21*b.m12 + a.m22*b.m22 + a.m23*b.m32;
	c.m23 = a.m21*b.m13 + a.m22*b.m23 + a.m23*b.m33;

	c.m31 = a.m31*b.m11 + a.m32*b.m21 + a.m33*b.m31;
	c.m32 = a.m31*b.m12 + a.m32*b.m22 + a.m33*b.m32;
	c.m33 = a.m31*b.m13 + a.m32*b.m23 + a.m33*b.m33;
}

// c = vec3 * mat3
void row3_mul_mat3(real3 c, const real3 a, const mat3 &m)
{
	c[0] = a[0] * m.m11 + a[1] * m.m21 + a[2] * m.m31;
	c[1] = a[0] * m.m12 + a[1] * m.m22 + a[2] * m.m32;
	c[2] = a[0] * m.m13 + a[1] * m.m23 + a[2] * m.m33;
}

/*
	Matrix 4x4
*/
/*
static const int m411 = 0;
static const int m412 = 1;
static const int m413 = 2;
static const int m414 = 3;
static const int m421 = 4;
static const int m422 = 5;
static const int m423 = 6;
static const int m424 = 7;
static const int m431 = 8;
static const int m432 = 9;
static const int m433 = 10;
static const int m434 = 11;
static const int m441 = 12;
static const int m442 = 13;
static const int m443 = 14;
static const int m444 = 15;
*/

void mat4_set_identity(mat4 &c)
{
	memset(&c, 0, sizeof(mat4));

	c.m11 = 1;
	c.m22 = 1;
	c.m33 = 1;
	c.m44 = 1;
}

void mat4_mul_scalar(mat4 &c, const mat4 &a, const real scalar)
{
	// row 1
	c.m11 = a.m11 * scalar;
	c.m12 = a.m12 * scalar;
	c.m13 = a.m13 * scalar;
	c.m14 = a.m14 * scalar;

	// row 2
	c.m21 = a.m21 * scalar;
	c.m22 = a.m22 * scalar;
	c.m23 = a.m23 * scalar;
	c.m24 = a.m24 * scalar;

	// row 3
	c.m31 = a.m31 * scalar;
	c.m32 = a.m32 * scalar;
	c.m33 = a.m33 * scalar;
	c.m34 = a.m34 * scalar;

	// row 4
	c.m41 = a.m41 * scalar;
	c.m42 = a.m42 * scalar;
	c.m43 = a.m43 * scalar;
	c.m44 = a.m44 * scalar;
}

void mat4_div_scalar(mat4 &c, const mat4&a, const real scalar)
{
	mat4_mul_scalar(c, a, 1.0f / scalar);
}

void mat4_transpose(mat4 &c, const mat4 &a)
{
	c.m11 = a.m11;
	c.m12 = a.m21;
	c.m13 = a.m31;
	c.m14 = a.m41;

	c.m21 = a.m12;
	c.m22 = a.m22;
	c.m23 = a.m32;
	c.m24 = a.m42;

	c.m31 = a.m13;
	c.m32 = a.m23;
	c.m33 = a.m33;
	c.m34 = a.m43;

	c.m41 = a.m14;
	c.m42 = a.m24;
	c.m43 = a.m34;
	c.m44 = a.m44;
}

real mat4_determinant(const mat4 &m)
{
	double value;
	value =
		m.m14 * m.m23 * m.m32 * m.m41 - m.m13 * m.m24 * m.m32 * m.m41 - m.m14 * m.m22 * m.m33 * m.m41 + m.m12 * m.m24 * m.m33 * m.m41 +
		m.m13 * m.m22 * m.m34 * m.m41 - m.m12 * m.m23 * m.m34 * m.m41 - m.m14 * m.m23 * m.m31 * m.m42 + m.m13 * m.m24 * m.m31 * m.m42 +
		m.m14 * m.m21 * m.m33 * m.m42 - m.m11 * m.m24 * m.m33 * m.m42 - m.m13 * m.m21 * m.m34 * m.m42 + m.m11 * m.m23 * m.m34 * m.m42 +
		m.m14 * m.m22 * m.m31 * m.m43 - m.m12 * m.m24 * m.m31 * m.m43 - m.m14 * m.m21 * m.m32 * m.m43 + m.m11 * m.m24 * m.m32 * m.m43 +
		m.m12 * m.m21 * m.m34 * m.m43 - m.m11 * m.m22 * m.m34 * m.m43 - m.m13 * m.m22 * m.m31 * m.m44 + m.m12 * m.m23 * m.m31 * m.m44 +
		m.m13 * m.m21 * m.m32 * m.m44 - m.m11 * m.m23 * m.m32 * m.m44 - m.m12 * m.m21 * m.m33 * m.m44 + m.m11 * m.m22 * m.m33 * m.m44;
	
	return (real)value;
}

// Create the 4x4 adjoint matrix 
/*
How do you find the adjoint of a 4x4 matrix?

Find the cofactor (determinant of the signed minor) of each entry, keeping in mind the sign array
(starting with + in the upper left corner):
[+ - + -]
[- + - +]
[+ - + -]
[- + - +]

//
//	11  12  13  14
//  21  22  23  24
//  31  32  33  34
//  41  42  43  44
//

// c = adjoint(a)
*/
void mat4_adjoint(mat4 &c, const mat4 &a)
{
	mat3 m3;
	mat4 m4;

	// Create matrix of minors
	// Apply +- pattern to create matrix of cofactors
	// 11
	m3.m11 = a.m22; m3.m12 = a.m23; m3.m13 = a.m24;
	m3.m21 = a.m32; m3.m22 = a.m33; m3.m23 = a.m34;
	m3.m31 = a.m42; m3.m32 = a.m43; m3.m33 = a.m44;
	m4.m11 = mat3_determinant(m3);

	// 12
	m3.m11 = a.m21; m3.m12 = a.m23; m3.m13 = a.m24;
	m3.m21 = a.m31; m3.m22 = a.m33; m3.m23 = a.m34;
	m3.m31 = a.m41; m3.m32 = a.m43; m3.m33 = a.m44;
	m4.m12 = -mat3_determinant(m3);

	// 13
	m3.m11 = a.m21; m3.m12 = a.m22; m3.m13 = a.m24;
	m3.m21 = a.m31; m3.m22 = a.m32; m3.m23 = a.m34;
	m3.m31 = a.m41; m3.m32 = a.m42; m3.m33 = a.m44;
	m4.m13 = mat3_determinant(m3);

	// 14
	m3.m11 = a.m21; m3.m12 = a.m22; m3.m13 = a.m23;
	m3.m21 = a.m31; m3.m22 = a.m32; m3.m23 = a.m33;
	m3.m31 = a.m41; m3.m32 = a.m42; m3.m33 = a.m43;
	m4.m14 = -mat3_determinant(m3);

	// 21
	m3.m11 = a.m12; m3.m12 = a.m13; m3.m13 = a.m14;
	m3.m21 = a.m32; m3.m22 = a.m33; m3.m23 = a.m34;
	m3.m31 = a.m42; m3.m32 = a.m43; m3.m33 = a.m44;
	m4.m21 = -mat3_determinant(m3);

	// 22
	m3.m11 = a.m11; m3.m12 = a.m13; m3.m13 = a.m14;
	m3.m21 = a.m31; m3.m22 = a.m33; m3.m23 = a.m34;
	m3.m31 = a.m41; m3.m32 = a.m43; m3.m33 = a.m44;
	m4.m22 = mat3_determinant(m3);

	// 23
	m3.m11 = a.m11; m3.m12 = a.m12; m3.m13 = a.m14;
	m3.m21 = a.m31; m3.m22 = a.m32; m3.m23 = a.m34;
	m3.m31 = a.m41; m3.m32 = a.m42; m3.m33 = a.m44;
	m4.m23 = -mat3_determinant(m3);

	// 24
	m3.m11 = a.m11; m3.m12 = a.m12; m3.m13 = a.m13;
	m3.m21 = a.m31; m3.m22 = a.m32; m3.m23 = a.m33;
	m3.m31 = a.m41; m3.m32 = a.m42; m3.m33 = a.m43;
	m4.m24 = mat3_determinant(m3);


	// 31
	m3.m11 = a.m12; m3.m12 = a.m13; m3.m13 = a.m14;
	m3.m21 = a.m22; m3.m22 = a.m23; m3.m23 = a.m24;
	m3.m31 = a.m42; m3.m32 = a.m43; m3.m33 = a.m44;
	m4.m31 = mat3_determinant(m3);

	// 32
	m3.m11 = a.m11; m3.m12 = a.m13; m3.m13 = a.m14;
	m3.m21 = a.m21; m3.m22 = a.m23; m3.m23 = a.m24;
	m3.m31 = a.m41; m3.m32 = a.m43; m3.m33 = a.m44;
	m4.m32 = -mat3_determinant(m3);

	// 33
	m3.m11 = a.m11; m3.m12 = a.m12; m3.m13 = a.m14;
	m3.m21 = a.m21; m3.m22 = a.m22; m3.m23 = a.m24;
	m3.m31 = a.m41; m3.m32 = a.m42; m3.m33 = a.m44;
	m4.m33 = mat3_determinant(m3);

	// 34
	m3.m11 = a.m11; m3.m12 = a.m12; m3.m13 = a.m13;
	m3.m21 = a.m21; m3.m22 = a.m22; m3.m23 = a.m23;
	m3.m31 = a.m41; m3.m32 = a.m42; m3.m33 = a.m43;
	m4.m34 = -mat3_determinant(m3);


	// 41
	m3.m11 = a.m12; m3.m12 = a.m13; m3.m13 = a.m14;
	m3.m21 = a.m22; m3.m22 = a.m23; m3.m23 = a.m24;
	m3.m31 = a.m32; m3.m32 = a.m33; m3.m33 = a.m34;
	m4.m41 = -mat3_determinant(m3);

	// 42
	m3.m11 = a.m11; m3.m12 = a.m13; m3.m13 = a.m14;
	m3.m21 = a.m21; m3.m22 = a.m23; m3.m23 = a.m24;
	m3.m31 = a.m31; m3.m32 = a.m33; m3.m33 = a.m34;
	m4.m42 = mat3_determinant(m3);

	// 43
	m3.m11 = a.m11; m3.m12 = a.m12; m3.m13 = a.m14;
	m3.m21 = a.m21; m3.m22 = a.m22; m3.m23 = a.m24;
	m3.m31 = a.m31; m3.m32 = a.m32; m3.m33 = a.m34;
	m4.m43 = -mat3_determinant(m3);

	// 44
	m3.m11 = a.m11; m3.m12 = a.m12; m3.m13 = a.m13;
	m3.m21 = a.m21; m3.m22 = a.m22; m3.m23 = a.m23;
	m3.m31 = a.m31; m3.m32 = a.m32; m3.m33 = a.m33;
	m4.m44 = mat3_determinant(m3);


	// create adjoint by transposing
	mat4_transpose(c, m4);
}

int mat4_inverse(mat4 &c, const mat4 &a)
{
	real determinant = mat4_determinant(a);
	if (determinant == 0) {
		return -1;
	}

	mat4 adjoint;
	mat4_adjoint(adjoint, a);

	mat4_div_scalar(c, adjoint, determinant);

	return 0;
}

// c = mat4 * mat4
void mat4_mul_mat4(mat4 &c, const mat4 &a, const mat4 &b)
{
	c.m11 = a.m11*b.m11 + a.m12*b.m21 + a.m13*b.m31 + a.m14*b.m41;
	c.m12 = a.m11*b.m12 + a.m12*b.m22 + a.m13*b.m32 + a.m14*b.m42;
	c.m13 = a.m11*b.m13 + a.m12*b.m23 + a.m13*b.m33 + a.m14*b.m43;
	c.m14 = a.m11*b.m14 + a.m12*b.m24 + a.m13*b.m34 + a.m14*b.m44;

	c.m21 = a.m21*b.m11 + a.m22*b.m21 + a.m23*b.m31 + a.m24*b.m41;
	c.m22 = a.m21*b.m12 + a.m22*b.m22 + a.m23*b.m32 + a.m24*b.m42;
	c.m23 = a.m21*b.m13 + a.m22*b.m23 + a.m23*b.m33 + a.m24*b.m43;
	c.m24 = a.m21*b.m14 + a.m22*b.m24 + a.m23*b.m34 + a.m24*b.m44;

	c.m31 = a.m31*b.m11 + a.m32*b.m21 + a.m33*b.m31 + a.m34*b.m41;
	c.m32 = a.m31*b.m12 + a.m32*b.m22 + a.m33*b.m32 + a.m34*b.m42;
	c.m33 = a.m31*b.m13 + a.m32*b.m23 + a.m33*b.m33 + a.m34*b.m43;
	c.m34 = a.m31*b.m14 + a.m32*b.m24 + a.m33*b.m34 + a.m34*b.m44;

	c.m41 = a.m41*b.m11 + a.m42*b.m21 + a.m43*b.m31 + a.m44*b.m41;
	c.m42 = a.m41*b.m12 + a.m42*b.m22 + a.m43*b.m32 + a.m44*b.m42;
	c.m43 = a.m41*b.m13 + a.m42*b.m23 + a.m43*b.m33 + a.m44*b.m43;
	c.m44 = a.m41*b.m14 + a.m42*b.m24 + a.m43*b.m34 + a.m44*b.m44;

}

// c = vec4 * mat4
void row4_mul_mat4(real4 c, const real *a, const mat4 &m)
{
	c[0] = a[0] * m.m11 + a[1] * m.m21 + a[2] * m.m31 + a[3] * m.m41;
	c[1] = a[0] * m.m12 + a[1] * m.m22 + a[2] * m.m32 + a[3] * m.m42;
	c[2] = a[0] * m.m13 + a[1] * m.m23 + a[2] * m.m33 + a[3] * m.m43;
	c[3] = a[0] * m.m14 + a[1] * m.m24 + a[2] * m.m34 + a[3] * m.m44;
}

void mat4_mul_col4(real4 c, const mat4 &m, const real *a)
{
	c[0] = m.m11*a[0] + m.m12 *a[1] + m.m13*a[2] + m.m14*a[3];
	c[1] = m.m21*a[0] + m.m22*a[1] + m.m23*a[2] + m.m24*a[3];
	c[2] = m.m31*a[0] + m.m32*a[1] + m.m33*a[2] + m.m34*a[3];
	c[3] = m.m41*a[0] + m.m42*a[1] + m.m43*a[2] + m.m44*a[3];
}


// Perform a transpose in place
void mat4_transpose(mat4 &m)
{
	float tmp = m.m21;
	m.m21 = m.m12;
	m.m12 = tmp;

	tmp = m.m32;
	m.m32 = m.m23;
	m.m23 = tmp;

	tmp = m.m31;
	m.m31 = m.m13;
	m.m13 = tmp;

	tmp = m.m41;
	m.m41 = m.m14;
	m.m14 = tmp;

	tmp = m.m42;
	m.m42 = m.m24;
	m.m24 = tmp;

	tmp = m.m43;
	m.m43 = m.m34;
	m.m34 = tmp;
}

/*
void mat4_setXVector(mat4 &mat, const Vec3f &v)
{
	mat.m11 = v.x;
	mat.m12 = v.y;
	mat.m13 = v.z;
}



void mat4_setYVector(mat4 &mat, const vec3 &v)
{
	mat.m21 = v.x;
	mat.m22 = v.y;
	mat.m23 = v.z;
}


void mat4_setZVector(mat4 &mat, const vec3 &v)
{
	mat.m31 = v.x;		// v.x;
	mat.m32 = v.y;		//  v.y;
	mat.m33 = v.z;		// v.z;
}

void mat4_setTranslation(mat4 &mat, const vec3 &t)
{
	mat.m41 = t.x;
	mat.m42 = t.y;
	mat.m43 = t.z;
}

// c = m * a
void mat4_mul_point(vec3 &c, const mat4 &m, const vec3 &a)
{
	c.x = m.m11*a[0] + m.m12 *a[1] + m.m13*a[2]; +m.m14;
	c.y = m.m21*a[0] + m.m22*a[1] + m.m23*a[2]; +m.m24;
	c.z = m.m31*a[0] + m.m32*a[1] + m.m33*a[2]; +m.m34;
}
*/
