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

#ifndef pipeline_opengl_h
#define pipeline_opengl_h 1

#include "graphicc.h"
#include "linearalgebra.h"

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

	void ogl_transform_point(real3 res, const mat4 &tmat, const real3 pt);
	void ogl_transform_rows(real *res, const mat4 &tmat, const real *inpts, const size_t nrows);

	void ogl_translate(mat4 &c, const real dx, const real dy, const real dz);

	void ogl_scale(mat4 &c, const real3 sxyz);


	void ogl_rotatex(mat4 &c, const real radians);
	void ogl_rotatey(mat4 &c, const real radians);
	void ogl_rotatez(mat4 &c, const real radians);
	void ogl_rotate_around_axis(mat4 &c, const real3 n, const real radians);
	void ogl_set_rotation(mat4 &c, const mat3 &rot);

	// Some render pipeline matrices
	// Setup the camera view (world to camera matrix)
	void ogl_lookat(mat4 &c, const real3 eye, const real3 lookAt, const real3 up);

	// Create a projection matrix
	void ogl_perspective(mat4 &c, const real zoomx, const real zoomy, const real near, const real far);
	void ogl_orthographic(mat4 &c, const real zoomx, const real zoomy, const real near, const real far);
	mat4 ogl_ortho(real left, real right, real bottom, real top, real n, real f);

	// Create matrix to map from normalized (projection) space to actual physical screen
	// screenx, screeny contain the transformed values
	// clipx, clipy represent the input point
	void ogl_map_to_window(real &screenx, real &screeny,
		const real clipx, const real clipy, const real clipw,
		const real winResx, const real winResy,
		const real winCenterx, const real winCentery);

	void ogl_create_ndc_to_window(const real Ds,
		const real winResx, const real winResy,
		const real Sx, const real Sy);

#ifdef __cplusplus
}
#endif

#define zoom_persp(fovrad) (1.0f / (tan(fovrad / 2.0f)))
#define fov_persp(zoom) (2.0 * atan(1.0f / zoom))

#define zoom_ortho(size) (2.0f/size)
#define size_ortho(zoom) (2.0f/zoom)
#endif

