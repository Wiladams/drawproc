#pragma once

/*
Reference
https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
https://gitlab.com/wikibooks-opengl/modern-tutorials/blob/master/obj-viewer/obj-viewer.cpp
*/

#include "geometry.h"

struct ArcBall
{
	int last_mx = 0;
	int last_my = 0;
	int cur_mx = 0;
	int cur_my = 0;
	int arcball_on = false;

	void onMouse(int button, int state, int x, int y);
	void onMotion(int x, int y);
	void onIdle();

	Vec3f get_arcball_vector(int x, int y);

};
