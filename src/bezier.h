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

#pragma once

#ifndef BEZIER_H
#define BEZIER_H

#include "graphicc.h"

#ifdef __cplusplus
extern "C" {
#endif

// calculate a factorial for an integer
//int intfactorial(const int n);

// computeCoefficients
// n represents the number of control points
// c is the array containing the calculated coefficients
//void bez_computeCoefficients(const int n, int * c);

// calculate a specific value along a one dimensional bezier curve
coord bez_point(const float u, const coord *controls, const int nControls, const int * c);

// calculate a specific point on a bezier curve
// u			- a value between 0.0 and 1.0, 0.0 is beginning of curve
//				1.0 is end if curve
// pt			- the structure which is filled in with the calculated point
// nControls	- The number of control points
// controls		- The array of control points
// c			- calculated coefficients
void bez3_point(const float u, Pt3 *pt, const int nControls, const Pt3 *controls, const int * c);

//
// general bezier curve calculation of arbitrary onts
void bez3_curve(const Pt3 *controls, const int nControls, const int m, Pt3 * curve);

// 
// quadratic bezier
// controls		- 3 control points
// m			- number of segments in curve
// curve		- array containing calculated points along curve
//
void bez3_quadratic(const Pt3 *controls, const int m, Pt3 *curve);


//
// Cubic Bezier
// 4 control points
//
// bezier_cubic_point
// 
// Calculates the value of the bezier curve at a given parametric position on the
// curve.  In this case, the curve is a single dimension.
DPROC_API float bezier_cubic_point(const float u, float p0, float p1, float p2, float p3);
DPROC_API float bezier_cubic_first_derivative(const float u, const float p0, const float p1, const float p2, const float p3);
DPROC_API float bezier_cubic_second_derivative(const float u, const float p0, const float p1, const float p2, const float p3);

// bez3_cubic
// Fill an array with points specified by the number of
// desired segments 'm'
// 
// controls		- 4 control points
// m			- number of segments in curve
// curve		- array which will contain calculated points along curve
//
// Note: Ideally, we'd be able to specify an amount of 'smoothness', and use more
// segments where the curve is changing rapidly, and fewer where it's changing slowly.
//
void bez3_cubic(const Pt3 *controls, const int m, Pt3 * curve);

#ifdef __cplusplus
}
#endif

#endif
