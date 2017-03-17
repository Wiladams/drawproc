#include "dproc_gl.h"


Matrix ogl_lookat(Vec3f &eye, Vec3f &center, Vec3f &up) 
{
	Vec3f z = (eye - center).normalize();
	Vec3f x = cross(up, z).normalize();
	Vec3f y = cross(z, x).normalize();
	Matrix Minv = Matrix::identity();
	Matrix Tr = Matrix::identity();
	for (int i = 0; i<3; i++) 
	{
		Minv[0][i] = x[i];
		Minv[1][i] = y[i];
		Minv[2][i] = z[i];
		Tr[i][3] = -center[i];
	}

	Matrix m = Minv*Tr;
	
	return m;
}


Matrix ogl_viewport(const int x, const int y, const int w, const int h)
{
	Matrix res = Matrix::identity();


	res[0][3] = x + w / 2.f;
	res[1][3] = y + h / 2.f;
	res[2][3] = 1.f;
	res[0][0] = w / 2.f;
	res[1][1] = h / 2.f;
	res[2][2] = 0;

	return res;
}

Matrix projection(float coeff)
{
	Matrix res = Matrix::identity();
	res[3][2] = coeff;

	return res;
}
