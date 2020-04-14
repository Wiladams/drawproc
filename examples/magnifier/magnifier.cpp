// magnifier.cpp : Defines the exported functions for the DLL application.
//

#include "drawproc.h"
#include "blockrenderer.h"



const int magWidth = 16;
const int magHeight = 16;
int magFactor = 2;

int overlayWidth = magWidth * magFactor;
int overlayHeight = magWidth * magFactor;

PSD srcImage;
pb_rgba overlayBuffer;
DPCLIPRECT magRect = { 0,0,magWidth,magHeight };
BlockRenderer blocky(magWidth, magHeight, magFactor, magFactor);

void mouseMoved()
{
	//magRect.x = CLAMP(mouseX, 0, srcImage->fb.frame.width - magWidth - 1);
	//magRect.y = CLAMP(mouseY, 0, srcImage->fb.frame.height - magHeight - 1);
}

void keyReleased()
{
	switch (keyCode) {
	case KC_DOWN:
		magFactor = magFactor / 2;
		break;

	case KC_UP:
		magFactor = magFactor * 2;
		break;
	}

	// free up the old overlayBuffer
	pb_rgba_free(&overlayBuffer);

	// allocate new data for overlay buffer
	overlayWidth = magWidth * magFactor;
	overlayHeight = magHeight * magFactor;
	blocky.fPixelWidth = magFactor;
	blocky.fPixelHeight = magFactor;

	pb_rgba_init(&overlayBuffer, overlayWidth, overlayHeight);
}

void drawOverlay()
{
	// Whenever the mouse moved, get new bits from the srcImage
	pb_rgba dstframe;
	//pb_rgba_get_frame(&srcImage->fb, magRect.x, magRect.y, magRect.width, magRect.height, &dstframe);

	// copy them over to the overlayImage


	for (int row = 0; row < magRect.height; row++) {
		for (int col = 0; col < magRect.width; col++) {
			uint32_t colorValue = pb_rgba_get_pixel(&dstframe, col, row);
			blocky.setPixel(&overlayBuffer, col, row, colorValue);
		}
	}

	int left = CLAMP(mouseX, 0, width - overlayBuffer.frame.width - 1);
	int top = CLAMP(mouseY, 0, height - overlayBuffer.frame.height - 1);

	raster_rgba_blit(gpb, left, top, &overlayBuffer);

}

void draw()
{
	//backgroundImage(srcImage);
	background(pLightGray);
	image(srcImage, 0, 0);

	drawOverlay();
}

void setup()
{
	// setup the overlay buffer, which will act as the frame buffer
	// for the expanded pixel drawing
	pb_rgba_init(&overlayBuffer, overlayWidth, overlayHeight);

	// load in the primary image to be displayed
	//srcImage = loadImage("moon-wide.ppm", nullptr);
	srcImage = GdLoadImageFromFile("moon-wide.ppm", 0);

	// set the size of the window based on the size of the image
	//createCanvas(srcImage->fb.frame.width, srcImage->fb.frame.height + 4 + 30);
	createCanvas(640, 480 + 4 + 30);

}


