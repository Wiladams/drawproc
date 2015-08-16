/*
Copyright 2015 William A Adams

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/



#include "raster_rgba.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Some helpful macros
#define lerp255(bg, fg, a) ((uint8_t)div255((fg*a+bg*(255-a))))

#define blend_color(bg, fg) RGBA(				\
	lerp255(GET_R(bg), GET_R(fg), GET_A(fg)), \
	lerp255(GET_G(bg), GET_G(fg), GET_A(fg)), \
	lerp255(GET_B(bg), GET_B(fg), GET_A(fg)), 255)


//
// Line Clipping in preparation for line drawing
//
typedef int OutCode;

static const int LN_INSIDE = 0; // 0000
static const int LN_LEFT = 1;   // 0001
static const int LN_RIGHT = 2;  // 0010
static const int LN_BOTTOM = 4; // 0100
static const int LN_TOP = 8;    // 1000

// Compute the bit code for a point (x, y) using the clip rectangle
// bounded diagonally by (xmin, ymin), and (xmax, ymax)

OutCode ComputeOutCode(const pb_rect &rct, const int x, const int y)
{
	OutCode code;
	double xmin = rct.x;
	double xmax = rct.x + rct.width - 1;
	double ymin = rct.y;
	double ymax = rct.y + rct.height - 1;

	code = LN_INSIDE;          // initialised as being inside of clip window

	if (x < xmin)           // to the left of clip window
		code |= LN_LEFT;
	else if (x > xmax)      // to the right of clip window
		code |= LN_RIGHT;
	if (y < ymin)           // below the clip window
		code |= LN_BOTTOM;
	else if (y > ymax)      // above the clip window
		code |= LN_TOP;

	return code;
}

// Cohen–Sutherland clipping algorithm clips a line from
// P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with 
// diagonal from (xmin, ymin) to (xmax, ymax).
bool clipLine(const pb_rect &bounds, int &x0, int &y0, int &x1, int &y1)
{
	// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
	OutCode outcode0 = ComputeOutCode(bounds, x0, y0);
	OutCode outcode1 = ComputeOutCode(bounds, x1, y1);
	bool accept = false;
	double xmin = bounds.x;
	double xmax = bounds.x + bounds.width - 1;
	double ymin = bounds.y;
	double ymax = bounds.y + bounds.height - 1;


	while (true) {
		if (!(outcode0 | outcode1)) { // Bitwise OR is 0. Trivially accept and get out of loop
			accept = true;
			break;
		}
		else if (outcode0 & outcode1) { // Bitwise AND is not 0. Trivially reject and get out of loop
			break;
		}
		else {
			// failed both tests, so calculate the line segment to clip
			// from an outside point to an intersection with clip edge
			double x, y;

			// At least one endpoint is outside the clip rectangle; pick it.
			OutCode outcodeOut = outcode0 ? outcode0 : outcode1;

			// Now find the intersection point;
			// use formulas y = y0 + slope * (x - x0), x = x0 + (1 / slope) * (y - y0)
			if (outcodeOut & LN_TOP) {           // point is above the clip rectangle
				x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
				y = ymax;
			}
			else if (outcodeOut & LN_BOTTOM) { // point is below the clip rectangle
				x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
				y = ymin;
			}
			else if (outcodeOut & LN_RIGHT) {  // point is to the right of clip rectangle
				y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
				x = xmax;
			}
			else if (outcodeOut & LN_LEFT) {   // point is to the left of clip rectangle
				y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
				x = xmin;
			}

			// Now we move outside point to intersection point to clip
			// and get ready for next pass.
			if (outcodeOut == outcode0) {
				x0 = x;
				y0 = y;
				outcode0 = ComputeOutCode(bounds, x0, y0);
			}
			else {
				x1 = x;
				y1 = y;
				outcode1 = ComputeOutCode(bounds, x1, y1);
			}
		}
	}

	return accept;
}


// Draw a span of pixels
// This is basically a SRCCOPY
void raster_rgba_span(pb_rgba *pb, const uint32_t x, const uint32_t y, const size_t len, const uint32_t *data)
{
	// get the pointer to the beginning of the row
	// in the destination
	uint32_t *dstPtr = &((uint32_t *)pb->data)[y*pb->pixelpitch + x];
	
	// Just loop over pointers doing basic copies
	size_t idx;
	for (idx = 0; idx < len; idx++)
	{
		*dstPtr = data[idx];
		dstPtr++;
	}
}

void raster_rgba_hline_fade(pb_rgba *pb, int x1, uint32_t color1, int x2, uint32_t color2, int y)
{
	int xdiff = x2 - x1;
	if (xdiff == 0)
		return;

	int c1rd = GET_R(color1);
	int c1gr = GET_G(color1);
	int c1bl = GET_B(color1);

	int c2rd = GET_R(color2);
	int c2gr = GET_G(color2);
	int c2bl = GET_B(color2);

	int rdx = c2rd - c1rd;
	int gdx = c2gr - c1gr;
	int bdx = c2bl - c1bl;

	float factor = 0.0f;
	float factorStep = 1.0f / (float)xdiff;

	// draw each pixel in the span
	for (int x = x1; x < x2; x++) {
		int rd = c1rd + (int)(rdx*factor);
		int gr = c1gr + (int)(gdx*factor);
		int bl = c1bl + (int)(bdx*factor);
		int fg = RGBA(rd, gr, bl, 255);
		pb_rgba_set_pixel(pb, x, y, fg);

		factor += factorStep;
	}
}

void raster_rgba_vline_fade(pb_rgba *pb, int y1, uint32_t color1, int y2, uint32_t color2, int x)
{
	int ydiff = y2 - y1;
	if (ydiff == 0)
		return;

	int c1rd = GET_R(color1);
	int c1gr = GET_G(color1);
	int c1bl = GET_B(color1);

	int c2rd = GET_R(color2);
	int c2gr = GET_G(color2);
	int c2bl = GET_B(color2);

	int rdx = c2rd - c1rd;
	int gdx = c2gr - c1gr;
	int bdx = c2bl - c1bl;

	float factor = 0.0f;
	float factorStep = 1.0f / (float)ydiff;

	// draw each pixel in the span
	for (int y = y1; y < y2; y++) {
		int rd = c1rd + (int)(rdx*factor);
		int gr = c1gr + (int)(gdx*factor);
		int bl = c1bl + (int)(bdx*factor);
		int fg = RGBA(rd, gr, bl, 255);
		pb_rgba_set_pixel(pb, x, y, fg);

		factor += factorStep;
	}
}

int raster_rgba_hline(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, uint32_t value)
{
	size_t terminus = x + length;
	terminus = terminus - x;

	unsigned int * data = &((unsigned int *)pb->data)[y*pb->pixelpitch+x];
	for (size_t idx = 0; idx < terminus; idx++)
	{
		*data = value;
		data++;
	}

	return 0;
}



int raster_rgba_hline_blend(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, uint32_t value)
{
	size_t terminus = x + length;
	terminus = terminus - x;

	unsigned int * data = &((unsigned int *)pb->data)[y*pb->pixelpitch + x];
	for (size_t idx = 0; idx < terminus; idx++)
	{
		int bg = *data;
		int fg = value;

		*data = blend_color(bg, fg);
		data++;
	}

	return 0;
}

int raster_rgba_vline(pb_rgba *pb, unsigned int x, unsigned int y, unsigned int length, uint32_t value)
{
	unsigned int * data = &((unsigned int *)pb->data)[y*(int)pb->frame.width + x];
	size_t count = 1;
	while (count <= length) {
		*data = value;
		data += pb->pixelpitch;
		count++;
	}

	return 0;
}



// Bresenham simple line drawing
void raster_rgba_line(pb_rgba *pb, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, uint32_t color)
{
	int dx, dy;
	int i;
	int sdx, sdy, dxabs, dyabs;
	unsigned int x, y, px, py;

	dx = x2 - x1;      /* the horizontal distance of the line */
	dy = y2 - y1;      /* the vertical distance of the line */
	dxabs = abs(dx);
	dyabs = abs(dy);
	sdx = sgn(dx);
	sdy = sgn(dy);
	x = dyabs >> 1;
	y = dxabs >> 1;
	px = x1;
	py = y1;

	pb_rgba_set_pixel(pb, x1, y1, color);

	if (dxabs >= dyabs) /* the line is more horizontal than vertical */
	{
		for (i = 0; i<dxabs; i++)
		{
			y += dyabs;
			if (y >= (unsigned int)dxabs)
			{
				y -= dxabs;
				py += sdy;
			}
			px += sdx;
			pb_rgba_set_pixel(pb, px, py, color);
		}
	}
	else /* the line is more vertical than horizontal */
	{
		for (i = 0; i<dyabs; i++)
		{
			x += dxabs;
			if (x >= (unsigned int)dyabs)
			{
				x -= dyabs;
				px += sdx;
			}
			py += sdy;
			pb_rgba_set_pixel(pb, px, py, color);
		}
	}
}

/*
	In this case, the pixel transfer operation is SRC_OVER instead of SRC_COPY
	Ideally, the cover would be fast enough that we would not need copy, but until
	then, it's ok to have to separate versions.

*/
void raster_rgba_line_cover(pb_rgba *pb, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, uint32_t color)
{
	int dx, dy;
	int i;
	int sdx, sdy, dxabs, dyabs;
	unsigned int x, y, px, py;

	dx = x2 - x1;      /* the horizontal distance of the line */
	dy = y2 - y1;      /* the vertical distance of the line */
	dxabs = abs(dx);
	dyabs = abs(dy);
	sdx = sgn(dx);
	sdy = sgn(dy);
	x = dyabs >> 1;
	y = dxabs >> 1;
	px = x1;
	py = y1;

	pb_rgba_set_pixel(pb, x1, y1, color);

	if (dxabs >= dyabs) /* the line is more horizontal than vertical */
	{
		for (i = 0; i<dxabs; i++)
		{
			y += dyabs;
			if (y >= (unsigned int)dxabs)
			{
				y -= dxabs;
				py += sdy;
			}
			px += sdx;
			pb_rgba_cover_pixel(pb, px, py, color);
		}
	}
	else /* the line is more vertical than horizontal */
	{
		for (i = 0; i<dyabs; i++)
		{
			x += dxabs;
			if (x >= (unsigned int)dyabs)
			{
				x -= dyabs;
				px += sdx;
			}
			py += sdy;
			pb_rgba_cover_pixel(pb, px, py, color);
		}
	}
}

void raster_rgba_blit(pb_rgba *pb, const int x, const int y, const pb_rgba *src)
{
	uint32_t *dstPtr = (uint32_t *)pb->data;
	uint32_t *srcPtr = (uint32_t *)src->data;

	dstPtr += y*pb->pixelpitch + x;

	// IMPROVE
	// use _span
	int srcrow = 0;
	for (srcrow = 0; srcrow < src->frame.height; srcrow++)
	{
		// for each row of the source
		// copy to the destination
		memcpy(dstPtr, srcPtr, src->pixelpitch * 4);
		dstPtr += pb->pixelpitch;
		srcPtr += src->pixelpitch;
	}
}

void raster_rgba_blend_alphamap(pb_rgba *pb, const int x, const int y, const unsigned char *bitmap, const int w, const int h, const uint32_t color)
{
	int xpos = x;
	int ypos = y;

	uint32_t *dstPtr = (uint32_t *)pb->data;
	uint8_t *srcPtr = (uint8_t *)bitmap;

	dstPtr += y*pb->pixelpitch + x;
	uint32_t *dstRowPtr = dstPtr;

	int srcrow = 0;
	for (srcrow = 0; srcrow < h; srcrow++)
	{
		xpos = x;

		for (int srccol = 0; srccol < w; srccol++)
		{
			if (*srcPtr > 0)
			{
				int dstColor = RGBA(GET_R(color), GET_G(color), GET_B(color), *srcPtr);
				pb_rgba_cover_pixel(pb, xpos, ypos, dstColor);
			}
			xpos++;
			srcPtr += 1;
		}

		ypos++;
	}
}

/*
	Bresenham ellipse drawing algorithm
	Only for the frame, not for filling
*/
typedef void(* EllipseHandler)(pb_rgba *pb, const uint32_t cx, const uint32_t cy, const unsigned int x, const unsigned int y, const uint32_t color);

inline void Plot4EllipsePoints(pb_rgba *pb, const uint32_t cx, const uint32_t cy, const unsigned int x, const unsigned int y, const uint32_t color)
{
	unsigned int lowx = cx - x;
	unsigned int maxx = cx + x;
	unsigned int lowy = cy - y;
	unsigned int maxy = cy + y;

	if (pb_rect_contains_point(pb->frame, maxx, maxy))
		pb_rgba_cover_pixel(pb, cx + x, cy + y, color);
	
	if (pb_rect_contains_point(pb->frame, lowx, maxy))
		pb_rgba_cover_pixel(pb, cx - x, cy + y, color);
	
	if (pb_rect_contains_point(pb->frame, lowx, lowy))
		pb_rgba_cover_pixel(pb, cx - x, cy - y, color);
	
	if (pb_rect_contains_point(pb->frame, maxx, lowy))
		pb_rgba_cover_pixel(pb, cx + x, cy - y, color);
}

inline void fill2EllipseLines(pb_rgba *pb, const uint32_t cx, const uint32_t cy, const unsigned int x, const unsigned int y, const uint32_t color)
{
	int x1 = cx - x;
	int y1 = cy+y;
	int x2 = cx + x;
	int y2 = cy+y;

	if (clipLine(pb->frame, x1, y1, x2, y2)) {
		raster_rgba_hline_blend(pb, x1, y1, x2-x1, color);
	}
	
	y1 = cy - y;
	y2 = cy - y;
	if (clipLine(pb->frame, x1, y1, x2, y2)) {
		raster_rgba_hline_blend(pb, x1, y1, x2 - x1, color);
	}

	//raster_rgba_hline_blend(pb, cx - x, cy - y, 2 * x, color);
}

void raster_rgba_ellipse(pb_rgba *pb, const uint32_t cx, const uint32_t cy, const size_t xradius, size_t yradius, const uint32_t color, EllipseHandler handler)
{
	int x, y;
	int xchange, ychange;
	int ellipseerror;
	int twoasquare, twobsquare;
	int stoppingx, stoppingy;

	twoasquare = 2 * xradius*xradius;
	twobsquare = 2 * yradius*yradius;

	x = xradius;
	y = 0;

	xchange = yradius*yradius*(1 - 2 * xradius);
	ychange = xradius*xradius;
	ellipseerror = 0;
	stoppingx = twobsquare*xradius;
	stoppingy = 0;

	// first set of points, sides
	while (stoppingx >= stoppingy)
	{
		handler(pb, cx, cy, x, y, color);
		y++;
		stoppingy += twoasquare;
		ellipseerror += ychange;
		ychange += twoasquare;
		if ((2 * ellipseerror + xchange) > 0) {
			x--;
			stoppingx -= twobsquare;
			ellipseerror += xchange;
			xchange += twobsquare;
		}
	}

	// second set of points, top and bottom
	x = 0;
	y = yradius;
	xchange = yradius*yradius;
	ychange = xradius*xradius*(1 - 2 * yradius);
	ellipseerror = 0;
	stoppingx = 0;
	stoppingy = twoasquare*yradius;

	while (stoppingx <= stoppingy) {
		handler(pb, cx, cy, x, y, color);
		x++;
		stoppingx += twobsquare;
		ellipseerror += xchange;
		xchange += twobsquare;
		if ((2 * ellipseerror + ychange) > 0) {
			y--;
			stoppingy -= twoasquare;
			ellipseerror += ychange;
			ychange += twoasquare;
		}
	}
}

void raster_rgba_ellipse_stroke(pb_rgba *pb, const uint32_t cx, const uint32_t cy, const size_t xradius, size_t yradius, const uint32_t color)
{
	raster_rgba_ellipse(pb, cx, cy, xradius, yradius, color, Plot4EllipsePoints);
}

void raster_rgba_ellipse_fill(pb_rgba *pb, const uint32_t cx, const uint32_t cy, const size_t xradius, size_t yradius, const uint32_t color)
{
	raster_rgba_ellipse(pb, cx, cy, xradius, yradius, color, fill2EllipseLines);
}
