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

DPROC_API bool clipLine(const DPCLIPRECT &bounds, int &x0, int &y0, int &x1, int &y1);

DPROC_API void raster_rgba_span(pb_rgba *pb, const DPCOORD x, const DPCOORD y, const size_t len, const DPPIXELVAL *data);

// SRCCOPY
DPROC_API int raster_rgba_hline(pb_rgba *pb, DPCOORD x, DPCOORD y, size_t length, const DPPIXELVAL value);
DPROC_API int raster_rgba_vline(pb_rgba *pb, DPCOORD x, DPCOORD y, size_t length, const DPPIXELVAL value);
DPROC_API void raster_rgba_line(pb_rgba *pb, DPCOORD x1, DPCOORD y1, DPCOORD x2, DPCOORD y2, const DPPIXELVAL value);

DPROC_API void raster_rgba_hline_fade(pb_rgba *pb, DPCOORD x1, DPPIXELVAL color1, DPCOORD x2, DPPIXELVAL color2, DPCOORD y);
DPROC_API void raster_rgba_vline_fade(pb_rgba *pb, DPCOORD y1, DPPIXELVAL color1, DPCOORD y2, DPPIXELVAL color2, DPCOORD x);

// SRCOVER
DPROC_API int raster_rgba_hline_blend(pb_rgba *pb, const DPCOORD x, const DPCOORD y, const size_t length, const DPPIXELVAL color);
DPROC_API void raster_rgba_line_cover(pb_rgba *pb, DPCOORD x1, DPCOORD y1, DPCOORD x2, DPCOORD y2, DPPIXELVAL color);



DPROC_API void raster_rgba_triangle_fill(pb_rgba *pb,
	const DPCOORD x1, const DPCOORD  y1,
	const DPCOORD  x2, const DPCOORD  y2,
	const DPCOORD  x3, const DPCOORD  y3,
	DPPIXELVAL color);


DPROC_API void raster_rgba_convex_polygon_fill(pb_rgba *pb, coord *verts, const int nverts, const DPCLIPRECT &clipRect, DPPIXELVAL color);

DPROC_API void raster_rgba_ellipse_fill(pb_rgba *pb, const DPCOORD cx, const uint32_t cy, const size_t xradius, size_t yradius, const DPPIXELVAL color);
DPROC_API void raster_rgba_ellipse_stroke(pb_rgba *pb, const DPCOORD cx, const uint32_t cy, const size_t xradius, size_t yradius, const DPPIXELVAL color);

DPROC_API void raster_rgba_blit(pb_rgba *pb, const DPCOORD x, const DPCOORD y, const pb_rgba *src);

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
DPROC_API void raster_rgba_blend_alphamap(pb_rgba *pb, const DPCOORD x, const DPCOORD y, const unsigned char *bitmap, const size_t w, const size_t h, const DPPIXELVAL color);

inline void raster_rgba_rect_fill(pb_rgba *pb, const DPCOORD x, const DPCOORD y, const size_t width, const size_t height, const DPPIXELVAL value)
{
	for (int idx = 0; idx < height; idx++){
		raster_rgba_hline(pb, x, y + idx, width, value);
	}
}

inline void raster_rgba_rect_fill_blend(pb_rgba *pb, const DPCOORD left, const DPCOORD top, const DPCOORD right, const DPCOORD bottom, const DPPIXELVAL value)
{
	size_t width = right - left+1;
	for (int y = top; y <= bottom; y++){
		raster_rgba_hline_blend(pb, left, y, width, value);
	}
}

#ifdef __cplusplus
}
#endif


#endif
