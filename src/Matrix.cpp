#include "Matrix.h"

Matrix4 Matrix4::operator *(const struct Matrix4 &B)
{
	static int N = 4;
	Matrix4 C;

	for (int i = 0; i < N; i++) {

		for (int k = 0; k < N; k++) {

			for (int j = 0; j < N; j++) 
			{
				C.a[i][j] += this->a[i][k] * B.a[k][j];
			}
		}
	}

	return C;
}

Matrix4 & Matrix4::operator *= (const double scalar)
{
	for (int idx = 0; idx < 16; idx++)
	{
		this->m[idx] = this->m[idx] * scalar;
	}

	return *this;
}

Matrix4 & Matrix4::operator /=(const double scalar)
{
	return *this *= (1 / scalar);
}

double Matrix4::determinant()
{
	double value;
	value =
		this->m14 * this->m23 * this->m32 * this->m41 - this->m13 * this->m24 * this->m32 * this->m41 - this->m14 * this->m22 * this->m33 * this->m41 + this->m12 * this->m24 * this->m33 * this->m41 +
		this->m13 * this->m22 * this->m34 * this->m41 - this->m12 * this->m23 * this->m34 * this->m41 - this->m14 * this->m23 * this->m31 * this->m42 + this->m13 * this->m24 * this->m31 * this->m42 +
		this->m14 * this->m21 * this->m33 * this->m42 - this->m11 * this->m24 * this->m33 * this->m42 - this->m13 * this->m21 * this->m34 * this->m42 + this->m11 * this->m23 * this->m34 * this->m42 +
		this->m14 * this->m22 * this->m31 * this->m43 - this->m12 * this->m24 * this->m31 * this->m43 - this->m14 * this->m21 * this->m32 * this->m43 + this->m11 * this->m24 * this->m32 * this->m43 +
		this->m12 * this->m21 * this->m34 * this->m43 - this->m11 * this->m22 * this->m34 * this->m43 - this->m13 * this->m22 * this->m31 * this->m44 + this->m12 * this->m23 * this->m31 * this->m44 +
		this->m13 * this->m21 * this->m32 * this->m44 - this->m11 * this->m23 * this->m32 * this->m44 - this->m12 * this->m21 * this->m33 * this->m44 + this->m11 * this->m22 * this->m33 * this->m44;

	return (double)value;
}

Matrix4 Matrix4::transpose()
{
	Matrix4 C;
	for (int col = 0; col < 4; col++)
	{
		for (int row = 0; row < 4; row++)
			C.a[row][col] = this->a[col][row];
	}

	return C;
}