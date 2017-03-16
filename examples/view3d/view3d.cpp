// view3d.cpp : Defines the exported functions for the DLL application.
//

#include "drawproc.h"

#include "pipeline_opengl.h"
#include <stdio.h>


mat4 ModelView;
mat4 Viewport;
mat4 Projection;

// World view points of triangle
// three points of an interesting triangle
Vec3f wpt1 = { 1, 1, 1 };
Vec3f wpt2 = { 200, 0, 0 };
Vec3f wpt3 = { 100, 300, 0 };

Vec3f v3 = { 1,1,1 };

// Points storing camera view of the world
Vec3f cp1, cp2, cp3;

Vec3f eye = { 1, 1, 3 };
Vec3f center = { 0, 0, 0 };
Vec3f up = { 0,1,0 };


// create a world to camera transformation matrix
void calcCamera()
{
	ModelView = ogl_lookat(eye, center, up);
	Viewport = ogl_viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
	Projection = projection(-1.0f / (eye - center).norm());

}

void setup()
{
	createCanvas(640, 480);
	calcCamera();
}

void keyReleased()
{
	switch (keyCode) {
	case KC_RIGHT:
		eye[0] = eye[0] + 2.0;
		break;
	case KC_LEFT:
		eye[0] = eye[0] - 2.0;
		break;
	case KC_UP:
		eye[2] = eye[2] + 2.0;
		break;
	case KC_DOWN:
		eye[2] = eye[2] - 2.0;
		break;
	}
	calcCamera();
}




void transformWorldToScreen(real &scrx, real &scry, const Vec3f &wpt) {
	char sbuff[256];
	textAlign(TX_LEFT, TX_TOP);
	fill(pBlack);

	// transform points from world view, to camera view
	Vec3f cp1;
	ogl_transform_point(cp1, Projection, wpt);

	
	text("World to Camera", 10, 80);
	sprintf_s(sbuff, "p1: %3.2f %3.2f %3.2f", cp1[0], cp1[1], cp1[2]);
	text(sbuff, 10, 100);
	

	// transform the camera points through 
	// orthographic projection
	mat4 orthom = ogl_ortho(0, width, height, 0, -1, 1);
	Vec3f npt1;
	ogl_transform_point(npt1, orthom, cp1);

	text("Camera to Normalized", 10, 180);
	sprintf_s(sbuff, "p1: %3.2f %3.2f %3.2f", npt1[0], npt1[1], npt1[2]);
	text(sbuff, 10, 200);


	// We now have points in in normalized space
	// convert to screen space
	// Create matrix to map from normalized (projection) space to actual physical screen
	// screenx, screeny contain the transformed values
	// clipx, clipy represent the input point

	ogl_map_to_window(scrx, scry,
		npt1[0], npt1[1], npt1[2],
		width, height,
		width / 2, height / 2);
}

void drawLine(const Vec3f a, const Vec3f b, const COLOR c)
{
	real scrx1 = 0;
	real scry1 = 0;
	real scrx2 = 0;
	real scry2 = 0;
	transformWorldToScreen(scrx1, scry1, a);
	transformWorldToScreen(scrx2, scry2, b);

	stroke(c);
	line(scrx1, scry1, scrx2, scry2);

}

void drawAxis()
{
	Vec3f origin = { 0,0,0 };
	Vec3f xaxis = { 100, 0, 0 };
	Vec3f yaxis = { 0,100,0 };
	Vec3f zaxis = { 0,0,100 };

	drawLine(origin, xaxis, pRed);
	drawLine(origin, yaxis, pGreen);
	drawLine(origin, zaxis, pBlue);
}

void drawTriangle()
{
	real trix1=0, triy1=0;
	real trix2=0, triy2=0;
	real trix3=0, triy3=0;

	transformWorldToScreen(trix1, triy1, wpt1);
	transformWorldToScreen(trix2, triy2, wpt2);
	transformWorldToScreen(trix3, triy3, wpt3);

	fill(pRed);
	triangle(trix1, triy1, trix2, triy2, trix3, triy3);

	text("Normalized to Screen", 10, 280);
	char sbuff[256];
	sprintf_s(sbuff, "p1: %8.2f %8.2f", trix1, triy1);
	text(sbuff, 10, 300);
	sprintf_s(sbuff, "p2: %8.2f %8.2f", trix2, triy2);
	text(sbuff, 10, 320);	
	sprintf_s(sbuff, "p3: %8.2f %8.2f", trix3, triy3);
	text(sbuff, 10, 340);
}

void draw()
{
	background(253);

	//drawTriangle();
	drawAxis();
}
