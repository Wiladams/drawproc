#include "pipeline_opengl.h"

#include <string.h>
#include <math.h>




// convenience
void ogl_transform_point(Vec3f &res, const mat4 &tmat, const Vec3f &pt)
{
	mat4_mul_point(res, tmat, pt);
}

// Multiply rows against transformation matrix
// assume columnar vectors on the right of the matrix
void ogl_transform_rows(real *res, const mat4 &tmat, const real *inpts, const size_t nrows)
{
	for (size_t idx = 0; idx < nrows; idx++)
	{
		mat4_mul_col4(&res[idx * 4], tmat, &inpts[idx * 4]);
	}
}

// translation
void ogl_translate(mat4 &c, const real dx, const real dy, const real dz)
{
	memset(&c, 0, sizeof(mat4));
	c.m11 = 1;
	c.m22 = 1;
	c.m33 = 1;
	c.m44 = 1;

	c.m41 = dx;
	c.m42 = dy;
	c.m43 = dz;
}

// Create a scaling matrix using all 3 axis
// scales along each of the axis from the origin
void ogl_scale(mat4 &c, const real3 sxyz)
{
	memset(&c, 0, sizeof(mat4));

	c.m11 = sxyz[0];
	c.m22 = sxyz[1];
	c.m33 = sxyz[2];
	c.m44 = 1;
}

// rotate around x axis
void ogl_rotatex(mat4 &c, const real radians)
{
	memset(&c, 0, sizeof(mat4));

	c.m11 = 1;
	c.m44 = 1;

	c.m22 = cos(radians);
	c.m23 = sin(radians);

	c.m32 = -sin(radians);
	c.m33 = cos(radians);
}

// rotate around y axis
void ogl_rotatey(mat4 &c, const real radians)
{
	memset(&c, 0, sizeof(mat4));

	c.m21 = 1;
	c.m44 = 1;

	c.m11 = cos(radians);
	c.m13 = -sin(radians);

	c.m31 = sin(radians);
	c.m33 = cos(radians);
}

// rotate around z axis
void ogl_rotatez(mat4 &c, const real radians)
{
	memset(&c, 0, sizeof(mat4));

	c.m33 = 1;
	c.m44 = 1;

	c.m11 = cos(radians);
	c.m12 = sin(radians);

	c.m21 = -sin(radians);
	c.m22 = cos(radians);
}

// Rotate around an arbitrary axis
// rotate around an axis with its center
// at the origin
void ogl_rotate_around_axis(mat4 &c, const real3 n, const real radians)
{
	real nx = n[0];
	real ny = n[1];
	real nz = n[2];
	real nx2 = n[0] * n[0];
	real ny2 = n[1] * n[1];
	real nz2 = n[2] * n[2];

	real cosR = cos(radians);
	real sinR = sin(radians);
	real onemincosR = 1 - cosR;
	real oneminsinR = 1 - sinR;

	// row 1
	c.m11 = nx2 * onemincosR + cosR;
	c.m12 = n[0] * n[1] * onemincosR + n[2] * sinR;
	c.m13 = n[0] * n[2] * onemincosR - n[1] * sinR;


	//row 2
	c.m21 = nx*ny*onemincosR - nz*sinR;
	c.m22 = ny2*onemincosR + cosR;
	c.m23 = ny*nz*onemincosR + nx*sinR;

	// row 3
	c.m31 = nx*nz*onemincosR + ny*sinR;
	c.m32 = ny*nz*onemincosR - nx*sinR;
	c.m33 = nz2*onemincosR + cosR;

	c.m44 = 1;
}




void ogl_set_rotation(mat4 &c, const mat3 &rot)
{
	c.m11 = rot.m11;
	c.m12 = rot.m12;
	c.m13 = rot.m13;

	c.m21 = rot.m21;
	c.m22 = rot.m22;
	c.m23 = rot.m23;

	c.m31 = rot.m31;
	c.m32 = rot.m32;
	c.m33 = rot.m33;

}

mat4 ogl_lookat(const Vec3f &eye, const Vec3f &center, const Vec3f &up)
{
	Vec3f z = (eye - center).normalize();
	Vec3f x = cross(up,z).normalize();
	Vec3f y = cross(z,x).normalize();

	mat4 Minv;
	mat4 Tr;
	mat4_set_identity(Minv);
	mat4_set_identity(Tr);

	mat4_setXVector(Minv, x);
	mat4_setYVector(Minv, y);
	mat4_setZVector(Minv, z);
	
	mat4_setTranslation(Tr, { -center[0] , -center[1],  -center[2] });

	mat4 res;
	mat4_mul_mat4(res, Minv, Tr);

	return res;
}



mat4 ogl_viewport(int x, int y, int w, int h)
{
	mat4 res;
	mat4_set_identity(res);
	
	res.m14 = x + w / 2.0f;
	res.m24 = y + h / 2.0f;
	res.m34 = 1.0f;
	res.m11 = w / 2.0f;
	res.m22 = h / 2.0f;
	res.m33 = 0;
	
	return res;
}

mat4 projection(float coeff) 
{
	mat4 res;
	mat4_set_identity(res);
	res.m43 = coeff;

	return res;
}


// Render pipeline transformation matrices
void ogl_perspective(mat4 &c, const real zoomx, const real zoomy, const real near, const real far)
{
	memset(&c, 0, sizeof(mat4));


	c.m11 = zoomx;
	c.m22 = zoomy;
	c.m33 = -(far + near) / (far - near);
	c.m43 = (-2 * near*far) / (far - near);
	c.m34 = -1;
}


void ogl_orthographic(mat4 &c, const real zoomx, const real zoomy, const real near, const real far)
{
	memset(&c, 0, sizeof(mat4));


	c.m11 = zoomx;
	c.m22 = zoomy;
	c.m33 = -(2) / (far - near);
	c.m43 = -((far + near) / (far - near));
	c.m44 = 1;
}

// Setup an OpenGL style orthographic projection
mat4 ogl_ortho(real left, real right, real bottom, real top, real z_near, real z_far)
{
	mat4 r;
	mat4_set_identity(r);
	r.m11 = 2.0f / (right - left);
	r.m22 = 2.0f / (top - bottom);
	r.m33 = -2.0f / (z_far - z_near);
	r.m41 = (right + left) / (left - right);
	r.m42 = (top + bottom) / (bottom - top);
	r.m43 = (z_near + z_far) / (z_near - z_far);

/*
	r.m11 = 2.0f / (right - left);           r.m12 = 0;								  r.m13 = 0;                 r.m14 = 0;
	r.m21 = 0;                               r.m22 = 2.0 / (top - bottom);            r.m23 = 0.0f;              r.m24 = 0.0f;
	r.m31 = 0.0f;                            r.m32 = 0.0f;                            r.m33 = 2.0f / (n - f);    r.m34 = 0.0f;
	r.m41 = (left + right) / (left - right); r.m42 = (bottom + top) / (bottom - top); r.m43 = (n + f) / (f - n); r.m44 = 1.0f;
*/
	return r;
}

// receives normalized coords
// clipx := -1 (left), 1 (right)
// clipy := -1 (bottom), 1 (top)
//
void ogl_map_to_window(real &screenx, real &screeny,
	const real clipx, const real clipy, const real clipw,
	const real winResx, const real winResy,
	const real winCenterx, const real winCentery)
{
	screenx = ((clipx*winResx) / (2 * clipw)) + winCenterx;
	screeny = -((clipy*winResy) / (2 * clipw)) + winCentery;
}

void ogl_create_ndc_to_window(const real Ds,
	const real winResx, const real winResy,
	const real Sx, const real Sy)
{
	real Wshalf = winResx / 2.0f;
	real Hshalf = winResy / 2.0f;
	real Dshalf = Ds / 2.0f;

	mat4 m = {
		Wshalf, 0, 0, 0,
		0, -Hshalf, 0, 0,
		0, 0, Dshalf, 0,
		Wshalf + Sx, Hshalf + Sy, Dshalf, 1
	};

}


