#include "BlockRenderer.h"

BlockRenderer::BlockRenderer(const int width, const int height, const int pixelWidth, const int pixelHeight)
	:fFrameWidth(width),
	fFrameHeight(height),
	fPixelWidth(pixelWidth),
	fPixelHeight(pixelHeight)
{
}

void BlockRenderer::setPixel(pb_rgba *pb, const int x, const int y, const int32_t value)
{
	int left = x*fPixelWidth;
	int top = y*fPixelHeight;

	// raw the pixel as a rectangle
	raster_rgba_rect_fill(pb, left, top, fPixelWidth, fPixelHeight, value);
}
