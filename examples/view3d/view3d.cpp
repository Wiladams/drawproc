// view3d.cpp : Defines the exported functions for the DLL application.
//



#include <vector>
#include <limits>
#include <iostream>
#include <stdio.h>

#include "drawproc.h"
#include "model.h"
#include "geometry.h"
#include "dproc_gl.h"
#include "diffuseshader.h"
#include "quaternion.h"


Matrix ModelView;
Matrix Projection; 
Matrix Viewport;

size_t modelIndex = 0;
Model *model = nullptr;
Model *floorModel = nullptr;

std::vector<Model *> models;
float *zbuffer = nullptr;

Vec3f light_dir(1, 1, 1);
Vec3f       eye(1, 1, 3);
Vec3f    center(0, 0, 0);
Vec3f        up(0, -1, 0);




void loadModels()
{
	floorModel = new Model("obj/floor.obj");

	models.push_back(new Model("obj/african_head/african_head.obj"));
	models.push_back(new Model("obj/diablo3_pose/diablo3_pose.obj"));
	models.push_back(new Model("obj/boggie/body.obj"));
	//models.push_back(new Model("obj/Vanquish/vanquish.obj"));

	model = models.at(0);
}

void onCameraChange()
{
	ModelView = ogl_lookat(eye, center, up);
	Projection = ogl_projection(-1.0f / (eye - center).norm());
}

void renderModel(Model *aModel)
{
	DiffuseShader shader(aModel, light_dir, ModelView, Projection, Viewport);

	for (int i = 0; i<aModel->nfaces(); i++) {
		for (int j = 0; j<3; j++) {
			shader.vertex(i, j);
		}
		ogl_triangle(Viewport, shader.varying_tri, shader, *gpb, zbuffer);
	}
}



void draw()
{
	background(120);

	ogl_clearzbuffer(zbuffer, width, height);


	renderModel(floorModel);
	renderModel(model);

	char str[255];
	sprintf_s(str, "delta: %d", mouseDelta);

	text(str, 10, 10);

}

void setup()
{
	createCanvas(800, 800);

	zbuffer = new float[width*height];
	loadModels();
	Viewport = ogl_viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
	onCameraChange();
	light_dir = proj<3>((Projection*ModelView*embed<4>(light_dir, 0.f))).normalize();

	//noLoop();
}

void keyReleased()
{
 
	switch (keyCode) {
	case KC_SPACE:
		modelIndex = modelIndex + 1;
		
		if (modelIndex == models.size()) 
		{
			modelIndex = 0;
		}
		
		model = models.at(modelIndex);

		break;

	case KC_RIGHT:
	case KC_LEFT:
		if (keyCode == KC_LEFT) {
			eye.x += 0.2f;
		}
		else if (keyCode == KC_RIGHT) {
			eye.x -= 0.2f;
			}

		onCameraChange();

		break;

	case KC_UP:
	case KC_DOWN:
		if (keyCode == KC_UP) {
			eye.y += 0.2f;
		}
		else if (keyCode == KC_DOWN) {
			eye.y -= 0.2f;
		}
		onCameraChange();
	}

	//draw();
}

void mouseWheel()
{
	if (mouseDelta > 0) 
		eye.z -= 0.2f;
	else if (mouseDelta < 0) 
		eye.z += 0.2f;

	mouseDelta = 0;
	onCameraChange();
}