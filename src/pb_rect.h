#pragma once

// pixel buffer rectangle
struct pb_rect {
	float x, y;
	float width, height;

	bool containsPoint(const float x, const float y);
	bool containsRect(const pb_rect &other);
	pb_rect intersection(const pb_rect &b);
	bool intersects(const pb_rect &b);
	void clear();
};
