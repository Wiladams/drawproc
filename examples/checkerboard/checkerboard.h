#pragma once

#include "AView.h"

class checkerboard : public AView
{
	size_t cols;
	size_t rows;
	DPPIXELVAL color1;
	DPPIXELVAL color2;

public:
	checkerboard(const int x, const int y, const size_t width, const size_t height, const size_t cols, const size_t rows, const DPPIXELVAL c1, const DPPIXELVAL c2)
		: AView(x,y,width,height),
		cols(cols), rows(rows),
		color1(c1), color2(c2)
	{
	}

	void Draw()
	{
		size_t tilewidth = fFrame.width / cols;
		size_t tileheight = (fFrame.height / rows);
		size_t boxwidth = tilewidth / 2;
		size_t boxheight = tileheight / 2;

		noStroke();

		rectMode(CORNER);
		for (size_t c = 0; c < cols; c++) {
			for (size_t r = 0; r < rows; r++) {
				fill(color1);
				rect(c*tilewidth, r*tileheight, boxwidth, boxheight);
				rect((c*tilewidth) + boxwidth, (r*tileheight) + boxheight, boxwidth, boxheight);

				fill(color2);
				rect((c*tilewidth) + boxwidth, r*tileheight, boxwidth, boxheight);
				rect(c*tilewidth, (r*tileheight) + boxheight, boxwidth, boxheight);
			}
		}
	}
};

