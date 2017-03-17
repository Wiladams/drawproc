#pragma once

#include "drawproc.h"
#include "geometry.h"
#include "IShader.h"

void ogl_triangle(Matrix &Viewport, mat<4, 3, float> &pts, IShader &shader, pb_rgba &image, float *zbuffer);
