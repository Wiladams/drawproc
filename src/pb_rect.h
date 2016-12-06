#pragma once

// pixel buffer rectangle
struct pb_rect {
	int x, y;
	int width, height;

	pb_rect() :
		x(0), y(0), 
		width(0), height(0)
	{}

	pb_rect(const int x, const int y, 
		const int gWidth, const int gHeight) :
		x(x), y(y),
		width(gWidth), height(gHeight)
	{}

	bool containsPoint(const float x, const float y);
	bool containsRect(const pb_rect &other);
	pb_rect intersection(const pb_rect &b);
	bool intersects(const pb_rect &b);
	void clear();
};

