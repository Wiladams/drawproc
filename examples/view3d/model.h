#pragma once
#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <string>

#include "geometry.h"
#include "tgaimage.h"
#include "GMesh.h"

class Model : GMesh {
private:
	TGAImage diffusemap_;
	TGAImage normalmap_;
	TGAImage specularmap_;
	void load_texture(std::string filename, const char *suffix, TGAImage &img);

public:
	Model(const char *filename);
	~Model();

	Vec3f normal(int iface, int nthvert);
	Vec3f normal(Vec2f uv);
	Vec3f vert(int i);
	Vec3f vert(int iface, int nthvert);
	Vec2f uv(int iface, int nthvert);
	TGAColor diffuse(Vec2f uv);
	float specular(Vec2f uv);
};
#endif //__MODEL_H__

