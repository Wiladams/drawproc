#pragma once

#include "geometry.h"
#include "drawproc.h"
#include "IShader.h"


//DPROC_EXPORT Matrix ModelView;
//DPROC_EXPORT Matrix Projection;
//DPROC_EXPORT Matrix Viewport;

// Some render pipeline matrices
// Setup the camera view (world to camera matrix)
Matrix ogl_lookat(Vec3f &eye, Vec3f &center, Vec3f &up);

// Create a projection matrix
Matrix ogl_projection(float coeff = 0.f); // coeff = -1/c

Matrix ogl_viewport(const int x, const int y, const int w, const int h);

void ogl_clearzbuffer(float *zbuffer, const size_t width, const size_t height);
void ogl_triangle(Matrix &Viewport, mat<4, 3, float> &pts, IShader &shader, pb_rgba &image, float *zbuffer);

