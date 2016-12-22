#pragma once

#include "drawproc.h"

struct checkerboard {
	size_t cols;
	size_t rows;
	COLOR color1;
	COLOR color2;

	checkerboard(const size_t cols, const size_t rows, const COLOR c1, const COLOR c2)
		: cols(cols), rows(rows),
		color1(c1), color2(c2)
	{
	}

	void Draw(pb_rgba *pb, const size_t width, const size_t height)
	{
		size_t tilewidth = width / cols;
		size_t tileheight = (height / rows);

		for (size_t c = 0; c < cols; c++) {
			for (size_t r = 0; r < rows; r++) {
				raster_rgba_rect_fill(pb, c*tilewidth, r*tileheight, tilewidth / 2, tileheight / 2, color1.value);
				raster_rgba_rect_fill(pb, (c*tilewidth) + tilewidth / 2, (r*tileheight) + tileheight / 2, tilewidth / 2, tileheight / 2, color1.value);

				raster_rgba_rect_fill(pb, (c*tilewidth) + tilewidth / 2, r*tileheight, tilewidth / 2, tileheight / 2, color2.value);
				raster_rgba_rect_fill(pb, c*tilewidth, (r*tileheight) + tileheight / 2, tilewidth / 2, tileheight / 2, color2.value);
			}
		}
	}
};

