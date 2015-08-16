

#include "raster_rgba.h"

int findTopmostVertex(coord *verts, const int numVerts)
{
	real ymin(32767);
	int vmin;
	coord *vertPtr = verts;

	for (int i = 0; i < numVerts; i++){
		if (vertPtr[1] < ymin)
		{
			ymin = vertPtr[1];
			vmin = i;
		}
		vertPtr += 2;
	}

	return vmin;
}

struct APolyDda {
	short vertIndex;
	short vertNext;
	real x;
	real dx;
	short ybeg;
	short yend;
	// for texture mapping
	//ATmapCoord tc;
	//ATmapCoord tcDelta;

	void setupPolyDda(const coord *pVerts, const int numVerts, short ivert, int dir)
	{
		vertIndex = ivert;
		vertNext = ivert + dir;
		if (vertNext < 0) {
			vertNext = numVerts - 1;
		}
		else if (vertNext == numVerts){
			vertNext = 0;
		}

		// set starting/ending ypos and current xpos
		ybeg = yend;
		yend = round(pVerts[vertNext*2+1]);
		x = pVerts[vertIndex*2+0];

		// Calculate fractional number of pixels to step in x (dx)
		real xdelta = pVerts[vertNext*2+0] -
			pVerts[vertIndex*2+0];
		int ydelta = yend - ybeg;
		if (ydelta > 0) {
			dx = xdelta / ydelta;
		}
		else {
			dx = 0;
		}
	}
};


void raster_rgba_convex_polygon_fill(pb_rgba *pb, coord *verts, const int nverts, const pb_rect &clipRect, int color)
{
	// find topmost vertex of the polygon
	int vmin = findTopmostVertex(verts, nverts);

	// set starting line
	APolyDda ldda, rdda;
	int y = int(verts[vmin*2+1]);
	ldda.yend = rdda.yend = y;

	// setup polygon scanner for left side, starting from top
	ldda.setupPolyDda(verts, nverts, vmin, +1);

	// setup polygon scanner for right side, starting from top
	rdda.setupPolyDda(verts, nverts, vmin, -1);

	//setColor(poly.colorNative);

	while (true)
	{
		if (y >= ldda.yend)
		{
			if (y >= rdda.yend)
			{
				if (ldda.vertNext == rdda.vertNext)	{ // if same vertex, then done
					break;
				}

				int vnext = rdda.vertNext - 1;

				if (vnext < 0) {
					vnext = nverts - 1;
				}

				if (vnext == ldda.vertNext)
				{
					break;
				}
			}
			ldda.setupPolyDda(verts, nverts, ldda.vertNext, +1);	// reset left side
		}

		// check for right dda hitting end of polygon side
		// if so, reset scanner
		if (y >= rdda.yend) {
			rdda.setupPolyDda(verts, nverts, rdda.vertNext, -1);
		}

		// fill span between two line-drawers, advance drawers when
		// hit vertices
		if (y >= clipRect.y) {
			raster_rgba_hline_blend(pb, round(ldda.x), y, round(rdda.x) - round(ldda.x), color);
		}

		ldda.x += ldda.dx;
		rdda.x += rdda.dx;

		// Advance y position.  Exit if run off its bottom
		y += 1;
		if (y >= clipRect.y + clipRect.height)
		{
			break;
		}
	}
}

