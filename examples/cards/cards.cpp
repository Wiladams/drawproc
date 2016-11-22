
#include "drawproc.h"

#include <stdio.h>

typedef enum {
	Clubs = 1,
	Spades = 2,
	Hearts = 3,
	Diamonds = 4
} suits;

PImage *kbImage;

unsigned int cardWidth = 73;
unsigned int cardHeight = 98;


void setup()
{
	kbImage = loadImage("cards.ppm", nullptr);
	size(kbImage->fb.frame.width, kbImage->fb.frame.height + 4 + 30);
	
	// setup text for drawing
	setFont(verdana18_bold);
	fill(0);
	textSize(20);
	textAlign(TX_LEFT, TX_TOP);

	background(pLightGray);

	//noLoop();

	draw();
}

/*
	getImage

	get an image object based on a specified suit and rank
	The suit values:
		Clubs - 1
		Spades - 2
		Hearts - 3
		Diamonds - 4

	Rank values
		run from 1 to 13
*/

void getImage(int suit, int rank, PImage *dstImage)
{
	unsigned int x = (rank-1) * cardWidth;
	unsigned int y = (suit - 1) * cardHeight;

	int gotit = pb_rgba_get_frame(&kbImage->fb, x, y, cardWidth, cardHeight, &dstImage->fb);

	return;
}

void draw()
{
	PImage dstImage;
	int iterations = 52;

	//background(pLightGray);
	//char debugstr[256];

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