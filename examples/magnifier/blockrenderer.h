#pragma once

#include "drawproc.h"

struct BlockRenderer
{
	int fFrameWidth;
	int fFrameHeight;
	int fPixelWidth;
	int fPixelHeight;

	BlockRenderer(const int width, const int height, const int pixelWidth, const int pixelHeight);

	void setPixel(pb_rgba *pb, const int x, const int y, const int32_t value);
};