/*
Copyright 2015 William A Adams

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/



#include "graphicc.h"

#include <stdlib.h>
#include <string.h>

/*
pb_rgba_init

Initialize a pixel buffer with data.  This routine will allocate the data
pointer and fill in the pixel pitch values.  After this routine returns,
the pixel buffer is ready to be drawn onto, and blitted onto other pixel
buffers.

*/

int pb_rgba_init(pb_rgba *fb, const unsigned int width, const unsigned int height)
{
	if (!fb) return -1;

	size_t pitch = width * 4;
	size_t datasize = height * pitch;

	fb->data = (unsigned char *)calloc(datasize, 1);
	fb->owndata = 1;

	fb->frame.x = 0;
	fb->frame.y = 0;
	fb->frame.width = width;
	fb->frame.height = height;

	fb->pixelpitch = width;
	fb->bitStride = width * 32;

	return 0;
}

int pb_rgba_free(pb_rgba *fb)
{
	if (!fb) return -1;
	if (!fb->data) return -1;

	if (fb->owndata) {
		free(fb->data);
	}

	fb->data = (unsigned char *)0;
	fb->frame.clear();

	return 0;
}

/*
pb_rgba_get_frame

Given a source pixel buffer, return a sub-section of that 
pixel buffer in the pf structure.  This does NOT make a copy
of the data.  Rather, it just points to the source image, and 
makes the appropriate adjustments in offsets and ownership.

thi is great for those situations where you have a big image 
that contains a group of images, and you need a sub-section, such
as a giant image of a deck of cards, and you need the image for 
a single card.  It would also work in cases where you have rendered
fonts into a single image, and you need to get sub-images for individual
glyphs.
*/
int pb_rgba_get_frame(pb_rgba *pb, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, pb_rgba *pf)
{
	// assume what's being asked for is already constrained
	pf->data = pb->data+((y*pb->pixelpitch+x)*sizeof(int));
	pf->owndata = 0;

	pf->frame.x = x;
	pf->frame.y = y;
	pf->frame.width = width;
	pf->frame.height = height;
	pf->pixelpitch = pb->pixelpitch;
	pf->bitStride = pb->bitStride;

	return 1;
}

void pb_rgba_cover_pixel(pb_rgba *pb, const unsigned int x, const unsigned int y, const uint32_t value)
{

	// Quick reject if the foreground pixel has both 0 opacity
	// and 0 for color component values
	if (0 == value) {
		return;
	}

	if (0 == GET_A(value)) {
		// Combine the colors, but don't
		// change the alpha of the background
	} else if (255 == GET_A(value)) {
		// The foreground opacity is full, so set
		// the color
		// and set the background alpha to full as well
		pb_rgba_set_pixel(pb, x, y, value);
	} else {
		// All other cases where doing a cover of something
		// other than full opacity
		uint8_t alpha = GET_A(value);
		int32_t dstPixel = pb_rgba_get_pixel(pb, x, y);


		int dstColor = RGBA(
			lerp255(GET_R(dstPixel), GET_R(value), alpha),
			lerp255(GET_G(dstPixel), GET_G(value), alpha), 
			lerp255(GET_B(dstPixel), GET_B(value), alpha), 
			lerp255(GET_A(dstPixel), GET_A(value), alpha)
		);
		pb_rgba_set_pixel(pb, x, y, dstColor);
		/*
		pix_rgba * B = (pix_rgba *)&pb->data[(y*(pb)->pixelpitch) + x];
		B->r = lerp255(B->r, GET_R(value), alpha);
		B->g = lerp255(B->g, GET_R(value), alpha);
		B->b = lerp255(B->b, GET_R(value), alpha);
		B->a = lerp255(B->a, GET_R(value), alpha);
		*/
	}
}



