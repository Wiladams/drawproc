#include "deck.h"

unsigned int cardWidth = 73;
unsigned int cardHeight = 98;

PImage * kbImage = loadImage("cards.ppm", nullptr);

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
	unsigned int x = (rank - 1) * cardWidth;
	unsigned int y = (suit - 1) * cardHeight;

	int gotit = pb_rgba_get_frame(&kbImage->fb, x, y, cardWidth, cardHeight, &dstImage->fb);

	return;
}
