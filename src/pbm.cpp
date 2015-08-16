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

#include <stdio.h>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>


#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4996)	// _CRT_SECURE_NO_WARNINGS (fopen) 
#endif

static const int MAXLINE = 100;

int CR = '\r';
int LF = '\n';

int readOneLine(FILE * f, char *buffer, const int size)
{
	int nchars = 0;
	int byteread;
	bool sawcr = false;

	while (nchars < size-1) {
		byteread = fgetc(f);

		if (byteread == EOF) break;

		if (byteread == LF) {
			break;
		}
		else if (byteread == CR) {
			// swallow it and continue on
			break;
			sawcr = true;
		}
		else {
			// Just a regular character, so save it
			buffer[nchars] = byteread;
			nchars = nchars + 1;
		}
	}

	buffer[nchars] = '\0';

	return nchars;
}

int getLine(FILE * f, char *buffer, const int size)
{
	int res;
	do {
		res = readOneLine(f, buffer, size);
	} while ((res != 0) && (buffer[0] == '#'));

	return res;
}


int PPM_read_binary(const char *filename, pb_rgba *fb)
{
	struct stat buf;
	int ret = stat(filename, &buf);
	//printf("res: %d, size: %lld\n", ret, buf.st_size);

	FILE * fp = fopen(filename, "rb");

	if (!fp) return -1;

	// read in the image header
	char marker[100];
	char sizes[100];
	char compsize[MAXLINE];


	ret = getLine(fp, marker, MAXLINE);
	ret = getLine(fp, sizes, MAXLINE);
	ret = getLine(fp, compsize, MAXLINE);


	char *strheight = strchr(sizes, ' ');
	if (!strheight) {
		return -1;
	}

	*strheight = '\0';
	strheight++;

	int imgWidth = atoi(sizes);
	int imgHeight = atoi(strheight);

	pb_rgba_init(fb, imgWidth, imgHeight);
															


	// read the individual pixel values in binary form
	unsigned char alpha = 255;
	for (int row = 0; row < imgHeight; row++) {
		for (int col = 0; col < imgWidth; col++) {
			int red = fgetc(fp);
			int green = fgetc(fp);
			int blue = fgetc(fp);
			pb_rgba_set_pixel(fb, col, row, RGBA(red, green, blue, alpha));
		}
	}

	fclose(fp);

	return 0;
}

/*
int read_PPM(const char *filename, pb_rgba *fb)
{

	return -1;
}
*/

int write_PPM_ascii(const char *filename, pb_rgba *fb)
{
	FILE * fp = fopen(filename, "wb");

	if (!fp) return -1;

	// write out the image header
	fprintf(fp, "P3\n%d %d\n255\n", int(fb->frame.width), int(fb->frame.height));

	// write the individual pixel values in binary form
	uint32_t * pixelPtr = (uint32_t *)fb->data;
	uint8_t rgb[3];

	for (int row = 0; row < fb->frame.height; row++) {
		for (int col = 0; col < fb->frame.width; col++){
			rgb[0] = GET_R(*pixelPtr);
			rgb[1] = GET_G(*pixelPtr);
			rgb[2] = GET_B(*pixelPtr);
			fprintf(fp, "%3d %3d %3d  ", rgb[0], rgb[1], rgb[2]);
		}
		pixelPtr += fb->pixelpitch;
		fprintf(fp,"\n");
	}


	fclose(fp);

	return 0;
}


int write_PPM_binary(const char *filename, pb_rgba *fb)
{
	FILE * fp = fopen(filename, "wb");
	
	if (!fp) return -1;

	// write out the image header
	fprintf(fp, "P6\n%d %d\n255\n", int(fb->frame.width), int(fb->frame.height));
	
	// write the individual pixel values in binary form
	uint32_t * pixelPtr = (uint32_t *)fb->data;
	uint8_t rgb[3];

	for (int row = 0; row < fb->frame.height; row++) {
		for (int col = 0; col < fb->frame.width; col++){
			rgb[0] = GET_R(pixelPtr[col]);
			rgb[1] = GET_G(pixelPtr[col]);
			rgb[2] = GET_B(pixelPtr[col]);
			fwrite((void *)&rgb[0], 3, 1, fp);
			//fwrite(&pixelPtr[col], 3, 1, fp);
		}
		pixelPtr += fb->pixelpitch;
	}


	fclose(fp);

	return 0;
}

#ifdef _WIN32
#pragma warning(pop)
#endif