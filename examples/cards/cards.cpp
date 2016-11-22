
#include "drawproc.h"

void setup()
{
	size(640, 360);
	stroke(pRed);
	rectMode(CENTER);
	background(127);

}

void draw()
{
	background(251);

	for (int i = 1; i <= 10000; i++)
	{
		double x = random(639);
		double y = random(359);


		point((int)x, (int)y);
	}
}