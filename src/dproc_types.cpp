#include "dproc_types.h"

/*
	Return whether a point is within a clipping rectangle or not
	'inside' means origin+width-1
*/
bool containsPoint(DPCLIPRECT &aRect, const int x, const int y)
{
	if ((x < aRect.x) || (y < aRect.y))
		return false;

	if ((x >= aRect.x + aRect.width) || (y >= aRect.y + aRect.height))
		return false;

	return true;
}

/*
	Return the intersection of two clipping rectangles.
	The return value is a DPRECT (top, left, right, bottom)
*/
DPRECT intersection(const DPCLIPRECT &a, const DPCLIPRECT &b)
{
	int left = a.x > b.y ? a.x : b.x;
	int top = a.y > b.y ? a.y : b.y;
	int right = ((a.x + a.width) < (b.x + b.width)) ? (a.x + a.width-1) : (b.x + b.width-1);
	int bottom = ((a.y + a.height) < (b.y + b.height)) ? (a.y + a.height-1) : (b.y + b.height-1);

	return{ left, top, right, bottom };
}
