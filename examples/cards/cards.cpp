#include "drawproc.h"
#include "deck.h"


COLOR backing = color(127, 255, 127);



void drawEntireDeck()
{
	PImage dstImage;
	int xoffset = 16;
	int yoffset = 104;

	// blank out the background to get a clean screen to
	// continue drawing
	background(pRed);

	int y = 4;
	for (int suit = 1; suit <= 4; suit++) {
		int x = 4;
		for (int rank = 1; rank <= 13; rank++) {
			// Get the image for the specific suit and rank
			getImage(suit, rank, &dstImage);

			// display the card image
			image(&dstImage, x, y);

			x = x + xoffset;
		}
		y = y + yoffset;
	}
}

void drawForeground()
{
}

void drawBackground()
{
	PImage dstImage;
	int iterations = 52;

	// blank out the background to get a clean screen to
	// continue drawing
	background(backing);

	for (int i = 1; i <= iterations; i++)
	{
		int suit = (int)random(1,4);
		int rank = (int)random(1,13);
		
		// Get the image for the specific suit and rank
		getImage(suit, rank, &dstImage);
		
		//get random location to display the card
		float randX = (float)random(width - cardWidth - 1);
		float randY = (float)random(height - cardHeight - 1);

		// display the card image
		image(&dstImage, randX, randY);
	}
}

void draw()
{
	//drawBackground();
	//drawForeground();
	
	drawEntireDeck();
}

void setup()
{
	size(960, 480);

	noLoop();

	// If looping, do the following
	//draw();
}
