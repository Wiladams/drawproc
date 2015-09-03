/*
Copyright 2015 William A Adams

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

//#pragma once

#ifndef RASTER_RGBA_H
#define RASTER_RGBA_H 1

#include "graphicc.h"


#ifdef __cplusplus
extern "C" {
#endif

DPROC_API bool clipLine(const pb_rect &bounds, int &x0, int &y0, int &x1, int &y1);

DPROC_API void raster_rgba_span(pb_rgba *pb, const uint32_t x, const uint32_t y, const size_t len, const uint32_t *data);

// SRCCOPY
DPROC_API int raster_rgba_hline(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, const uint32_t value);
DPROC_API int raster_rgba_vline(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, const uint32_t value);
DPROC_API void raster_rgba_line(pb_rgba *pb, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, const uint32_t value);

DPROC_API void raster_rgba_hline_fade(pb_rgba *pb, int x1, int color1, int x2, int color2, int y);
DPROC_API void raster_rgba_vline_fade(pb_rgba *pb, int y1, int color1, int y2, int color2, int x);

// SRCOVER
DPROC_API int raster_rgba_hline_blend(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, const uint32_t color);
DPROC_API void raster_rgba_line_cover(pb_rgba *pb, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, uint32_t color);



DPROC_API void raster_rgba_triangle_fill(pb_rgba *pb,
	const unsigned int x1, const unsigned int  y1,
	const unsigned int  x2, const unsigned int  y2,
	const unsigned int  x3, const unsigned int  y3,
	int color);


DPROC_API void raster_rgba_convex_polygon_fill(pb_rgba *pb, coord *verts, const int nverts, const pb_rect &clipRect, int color);

DPROC_API void raster_rgba_ellipse_fill(pb_rgba *pb, const uint32_t cx, const uint32_t cy, const size_t xradius, size_t yradius, const uint32_t color);
DPROC_API void raster_rgba_ellipse_stroke(pb_rgba *pb, const uint32_t cx, const uint32_t cy, const size_t xradius, size_t yradius, const uint32_t color);

DPROC_API void raster_rgba_blit(pb_rgba *pb, const int x, const int y, const pb_rgba *src);

/*
	raster_rgba_blend_alphamap
	The alpha map has 8 bits per pixel.  Each pixel represents the alpha
	value for the fixed color value.  If the alpha is 0, the pixel is not rendered.

	The bitmap has a byte row stride equal to the width indicated.
	x - x position within destination
	y - y position within destination
	bitmap - source alpha values
	w - width of bitmap, in bytes
	h - height of alphamap, in bytes
	color - base color value
*/
DPROC_API void raster_rgba_blend_alphamap(pb_rgba *pb, const int x, const int y, const unsigned char *bitmap, const int w, const int h, const uint32_t color);

inline void raster_rgba_rect_fill(pb_rgba *pb, const int x, const int y, const int width, const int height, const uint32_t value)
{
	for (int idx = 0; idx < height; idx++){
		raster_rgba_hline(pb, x, y + idx, width, value);
	}
}

inline void raster_rgba_rect_fill_blend(pb_rgba *pb, const int x, const int y, const int width, const int height, const uint32_t value)
{
	for (int idx = 0; idx < height; idx++){
		raster_rgba_hline_blend(pb, x, y + idx, width, value);
	}
}

#ifdef __cplusplus
}
#endif


#endif
