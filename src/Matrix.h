
#include "dproc_config.h"






// Use various representations of the matrix
// for different patterns of access
// For MSVC, we need to disable the warning  about nameless struct/union
#pragma warning(push)
#pragma warning(disable:4201)
struct Matrix4
{
	union {
		struct {
			__m128 m1, m2, m3, m4;
		};

		struct {
			float m11, m12, m13, m14;
			float m21, m22, m23, m24;
			float m31, m32, m33, m34;
			float m41, m42, m43, m44;
		};

		struct {
			float m[16];
		};

		struct {
			float a[4][4];
		};
	};

	Matrix4 operator *(const struct Matrix4 &other);

	Matrix4& operator *= (const double s);
	Matrix4& operator /= (const double s);

	// Some useful functions
	double determinant();
	Matrix4 transpose();
};
#pragma warning(pop)

/*

// identity, also Kronecker delta
void mat4_set_identity(mat4 &c);

real mat4_determinant(const mat4 &m);
void mat4_adjoint(mat4 &c, const mat4 &a);
int mat4_inverse(mat4 &c, const mat4 &a);
void mat4_mul_mat4(mat4 &c, const mat4 &a, const mat4 &b);

// assuming row vectors, multiply with matrix on the right
// c = a * m;
void row4_mul_mat4(real4 c, const real *a, const mat4 &m);

// assuming a column vector, multiply with matrix on left
// c = m * a;
void mat4_mul_col4(real4 c, const mat4 &m, const real *a);
*/
