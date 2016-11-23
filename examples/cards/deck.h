#pragma once

#include "drawproc.h"

extern unsigned int cardWidth;
extern unsigned int cardHeight;

// symbolic names for the suits
typedef enum {
	Clubs = 1,
	Spades = 2,
	Hearts = 3,
	Diamonds = 4
} suits;

// symbolic names for ranks
typedef enum {
	ace = 1,
	two = 2,
	three = 3,
	four = 4,
	five = 5,
	six = 6,
	seven = 7,
	eight = 8,
	nine = 9,
	ten = 10,
	jack = 11,
	queen = 12,
	king = 13
} ranks;

void getImage(int suit, int rank, PImage *dstImage);
