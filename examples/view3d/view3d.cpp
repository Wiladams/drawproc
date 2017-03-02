// view3d.cpp : Defines the exported functions for the DLL application.
//

#include "drawproc.h"
#include "pipeline_opengl.h"
#include <stdio.h>

mat4 cmat;

void setup()
{
	createCanvas(640, 480);

	// create a world to camera transformation matrix
	real3 eye = { 10, -100, 10 };
	real3 lookAt = { 0, 0, 0 };
	real3 up = { 0,0,1 };

	ogl_lookat(cmat, eye, lookAt, up);
}

void draw()
{
	// three points of an interesting triangle
	real3 pt1 = { -100, -10, -10 };
	real3 pt2 = { 100, -10, -10 };
	real3 pt3 = { 0, 200, 10 };

	// transform the points
	real3 p1, p2, p3;
	ogl_transform_point(p1, cmat, pt1);
	ogl_transform_point(p2, cmat, pt2);
	ogl_transform_point(p3, cmat, pt3);

	// We now have points in camera space
	// convert to screen space
	background(253);
	stroke(pBlack);
	fill(pBlack);
	textAlign(TX_LEFT, TX_TOP);
	char sbuff[256];
	sprintf_s(sbuff, "p1: %3.2f %3.2f %3.2f", p1[0], p1[1], p1[2]);
	text(sbuff, 10, 100);
	sprintf_s(sbuff, "p2: %f %f %f", p2[0], p2[1], p2[2]);
	text(sbuff, 10, 120);
	sprintf_s(sbuff, "p3: %f %f %f", p3[0], p3[1], p3[2]);
	text(sbuff, 10, 140);

	//fill(pRed);
	//triangle(x1, y1, x2, y2, x3, y3);
}
