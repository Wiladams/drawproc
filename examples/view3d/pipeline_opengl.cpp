#include "pipeline_opengl.h"
#include "linearalgebra.h"

#include <string.h>
#include <math.h>


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

void mat4_setXVector(mat4 &mat, const vec3 &v)
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

// convenience
void ogl_transform_point(vec3 &res, const mat4 &tmat, const vec3 &pt)
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

void ogl_lookat(mat4 &mat, const vec3 &eye, const vec3 &at, const vec3 &up)
{
	vec3 f = eye - at;
	f.normalize();

	//real3 rN;
	//real3 r;
	//real3_cross(rN, up, f);
	//real3_normalize(r, rN);
	vec3 r = up.cross(f);
	r.normalize();

	//real3 u;
	//real3_cross(u, f, r);
	vec3 u = f.cross(r);

	//mat4 mat;
	mat4_set_identity(mat);
	mat4_setXVector(mat, r);
	mat4_setYVector(mat, u);
	mat4_setZVector(mat, f);
	mat4_transpose(mat);
	
	vec3 tran = { -r.dot(eye) , -u.dot(eye),  -f.dot(eye) };

	mat4_setTranslation(mat,tran);
}

/*
void ogl_lookat(mat4 &mat, const real3 eye, const real3 center, const real3 up)
{
	real * m = (real*)&mat;

	real3 tmpreal3;
	real3 f;
	real3 upN;

	real3 s;
	//real *s = &m[0];

	real3 u;
	//real *u = &m[4];

	// Compute our new look at vector, which will be
	//   the new negative Z axis of our transformed object.
	real3_sub(tmpreal3, center, eye);
	real3_normalize(f, tmpreal3);
	real3_normalize(upN, up);

	real3_cross(s, f, upN);
	real3_cross(u, s, f);

	m[0] = s[0];    m[1] = s[1];     m[2] = s[2];     m[3] = 0;
	m[4] = u[0];    m[5] = u[1];     m[6] = u[2];     m[7] = 0;
	m[8] = -f[0];   m[9] = -f[1];    m[10] = -f[2];    m[11] = 0;
	m[12] = -eye[0]; m[13] = -eye[1];  m[14] = -eye[2];  m[15] = 1;


}
*/

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

