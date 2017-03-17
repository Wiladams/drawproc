#pragma once

#include "geometry.h"

// Some render pipeline matrices
// Setup the camera view (world to camera matrix)
Matrix ogl_lookat(Vec3f &eye, Vec3f &center, Vec3f &up);

// Create a projection matrix
Matrix projection(float coeff = 0.f); // coeff = -1/c

Matrix ogl_viewport(const int x, const int y, const int w, const int h);


