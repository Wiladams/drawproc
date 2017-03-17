#pragma once

#include "geometry.h"
#include "TGAImage.h"

struct IShader {
	virtual ~IShader() {};
	virtual Vec4f vertex(int iface, int nthvert) = 0;
	virtual bool fragment(Vec3f bar, TGAColor &color) = 0;
};
