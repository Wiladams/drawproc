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


#include "bezier.h"


#include <math.h>
#include <stdlib.h>


// calculate a factorial for an integer
static int intfactorial(const int n)
{
	int c = 1;
	for (int k = 1; k <= n; k++)
		c *= k;
	
	return c;
}

// computeCoefficients
// n represents the number of control points
// c is the array containing the calculated coefficients
static void bez_computeCoefficients(const int n, int * c)
{
	int k, i;

	for (k = 0; k <= n; k++)
	{
		// compute n!/(k!(n-k)!)
		c[k] = 1;
		for (i = n; i >= k + 1; i--)
		{
			c[k] *= i;
		}

		for (i = n - k; i >= 2; i--)
		{
			c[k] /= i;
		}
	}
}

coord bez_point(const float u, const coord *controls, const int nControls, const int * c)
{
	int k;
	int n = nControls - 1;
	float blend;

	coord res = 0.0;

	// Add in influence of each control point
	for (k = 0; k < nControls; k++){
		blend = (float)c[k] * powf(u, k) *powf(1.0f - u, n - k);
		res = res + controls[k] * blend;
	}
	return res;
}

void bez3_point(Pt3 * pt, const float u, const int nControls, const Pt3 *controls, const int * c)
{
	int k;
	int n = nControls - 1;
	float blend;

	pt->x = 0.0;	// x
	pt->y = 0.0;	// y
	pt->z = 0.0;	// z
	
	// Add in influence of each control point
	for (k = 0; k < nControls; k++){
		blend = (float)c[k] * powf(u, (float)k) *powf(1.0f - u, n - k);
		pt->x += controls[k].x * blend;
		pt->y += controls[k].y * blend;
		pt->z += controls[k].z * blend;
	}
}

void bez3_curve(const Pt3 *controls, const int nControls, const int m, Pt3 * curve)
{
	// create space for the coefficients
	int * c = (int *)malloc(nControls * sizeof(int));
	int i;

	bez_computeCoefficients(nControls - 1, c);
	for (i = 0; i <= m; i++) {
		bez3_point(&curve[i], i / (float)m, nControls, controls, c);
	}
	free(c);	
}

void bez3_quadratic(const Pt3 *controls, const int m, Pt3 * curve)
{
	bez3_curve(controls, 3, m, curve);
}

void bez3_cubic(const Pt3 *controls, const int m, Pt3 * curve)
{
	bez3_curve(controls, 4, m, curve);
	/*
	//int nControls = 4;
	int i = 0;

	for (i = 0; i <= m; i++) {
		float u = i / (float)m;
		float oneminusu = 1 - u;
		float BEZ03 = powf(oneminusu, 3);				// (1-u)^3
		float BEZ13 = 3 * u*(oneminusu * oneminusu);	// 3u(1-u)^2
		float BEZ23 = 3 * u*u * oneminusu;				// 3u^2(1-u)
		float BEZ33 = u*u*u;							// u^3

		curve[i].x = 0.0;	// x
		curve[i].y = 0.0;	// y
		curve[i].z = 0.0;	// z

		//computeBezierCubicPoint(u, &curve[i], controls);
		// Add in influence of each control point
		curve[i].x += controls[0].x * BEZ03;
		curve[i].y += controls[0].y * BEZ03;
		curve[i].z += controls[0].z * BEZ03;

		curve[i].x += controls[1].x * BEZ13;
		curve[i].y += controls[1].y * BEZ13;
		curve[i].z += controls[1].z * BEZ13;

		curve[i].x += controls[2].x * BEZ23;
		curve[i].y += controls[2].y * BEZ23;
		curve[i].z += controls[2].z * BEZ23;

		curve[i].x += controls[3].x * BEZ33;
		curve[i].y += controls[3].y * BEZ33;
		curve[i].z += controls[3].z * BEZ33;
	}
	*/
}