#pragma once

#include "AView.h"

class checkerboard : public AView
{
	size_t cols;
	size_t rows;
	COLOR color1;
	COLOR color2;

public:
	checkerboard(const int x, const int y, const size_t width, const size_t height, const size_t cols, const size_t rows, const COLOR c1, const COLOR c2)
		: AView(x,y,width,height),
		cols(cols), rows(rows),
		color1(c1), color2(c2)
	{
	}

	void Draw()
	{
		size_t tilewidth = fFrame.width / cols;
		size_t tileheight = (fFrame.height / rows);
		noStroke();

		for (size_t c = 0; c < cols; c++) {
			for (size_t r = 0; r < rows; r++) {
				fill(color1);
				rect(c*tilewidth, r*tileheight, tilewidth / 2, tileheight / 2);
				//rect((c*tilewidth) + tilewidth / 2, (r*tileheight) + tileheight / 2, tilewidth / 2, tileheight / 2);

				fill(color2);
				rect((c*tilewidth) + tilewidth / 2, r*tileheight, tilewidth / 2, tileheight / 2);
				//rect(c*tilewidth, (r*tileheight) + tileheight / 2, tilewidth / 2, tileheight / 2);
			}
		}
	}
};

