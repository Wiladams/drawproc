#include "pb_rect.h"

bool pb_rect::containsPoint(const float x, const float y)
{
	if ((x < this->x) || (y < this->y))
		return false;

	if ((x >= this->x + this->width) || (y >= this->y + this->height))
		return false;

	return true;
}

bool pb_rect::containsRect(const pb_rect &other)
{
	if (!containsPoint(other.x, other.y))
	{
		return false;
	}

	if (!containsPoint(other.x + other.width - 1, other.y + other.height - 1))
	{
		return false;
	}

	return true;
}

// return the intersection of rectangles a and b
// if there is no intersection, one or both of width and height
// will be == zero
pb_rect pb_rect::intersection(const pb_rect &b)
{


	float x = this->x > b.x ? this->x : b.x;
	float y = this->y > b.y ? this->y : b.y;
	float right = ((this->x + this->width) < (b.x + b.width)) ? (this->x + this->width) : (b.x + b.width);
	float bottom = ((this->y + this->height) < (b.y + b.height)) ? (this->y + this->height) : (b.y + b.height);

	float width = ((right - x) > 0) ? (right - x) : 0;
	float height = ((bottom - y) > 0) ? (bottom - y) : 0;
	
	return{ x, y, width, height };
/*
	pb_rect c;
	c.x = x;
	c.y = y;
	c.width = width;
	c.height = height;

	return c;
*/
	
}

// returns whether two rectangles intersect
// if the intersection would result in a rectangle with zero
// width or height, the answer is 'false'
bool pb_rect::intersects(const pb_rect &b)
{
	return (this->x < (b.x + b.width - 1)) &&
		(b.x < (this->x + this->width - 1)) &&
		(this->y < (b.y + b.height - 1)) &&
		(b.y < (this->y + this->height - 1));
}

void pb_rect::clear()
{
	this->x = 0;
	this->y = 0;
	this->width = 0;
	this->height = 0;
}