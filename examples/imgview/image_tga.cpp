/* ---------------------------------------------------------------------------
* Truevision Targa Reader/Writer
* Copyright (C) 2001-2003 Emil Mikulic.
*
* Source and binary redistribution of this code, with or without changes, for
* free or for profit, is allowed as long as this copyright notice is kept
* intact.  Modified versions must be clearly marked as modified.
*
* This code is provided without any warranty.  The copyright holder is
* not liable for anything bad that might happen as a result of the
* code.
* -------------------------------------------------------------------------*/


#define TGA_KEEP_MACROS /* BIT, htole16, letoh16 */
#include "image_tga.h"
#include <stdlib.h>
#include <string.h> /* memcpy, memcmp */

#define SANE_DEPTH(x) ((x) == 8 || (x) == 16 || (x) == 24 || (x) == 32)
#define UNMAP_DEPTH(x)            ((x) == 16 || (x) == 24 || (x) == 32)

static const char tga_id[] =
"\0\0\0\0" /* extension area offset */
"\0\0\0\0" /* developer directory offset */
"TRUEVISION-XFILE.";

static const size_t tga_id_length = 26; /* tga_id + \0 */



										/* helpers */
static tga_result tga_read_rle(tga_image *dest, FILE *fp);
static tga_result tga_write_row_RLE(FILE *fp,
	const tga_image *src, const uint8_t *row);
typedef enum { RAW, RLE } packet_type;
static packet_type rle_packet_type(const uint8_t *row, const uint16_t pos,
	const uint16_t width, const uint16_t bpp);
static uint8_t rle_packet_len(const uint8_t *row, const uint16_t pos,
	const uint16_t width, const uint16_t bpp, const packet_type type);



uint8_t tga_get_attribute_bits(const tga_image *tga)
{
	return tga->image_descriptor & TGA_ATTRIB_BITS;
}

int tga_is_right_to_left(const tga_image *tga)
{
	return (tga->image_descriptor & TGA_R_TO_L_BIT) != 0;
}

int tga_is_top_to_bottom(const tga_image *tga)
{
	return (tga->image_descriptor & TGA_T_TO_B_BIT) != 0;
}

int tga_is_colormapped(const tga_image *tga)
{
	return (
		tga->image_type == TGA_IMAGE_TYPE_COLORMAP ||
		tga->image_type == TGA_IMAGE_TYPE_COLORMAP_RLE
		);
}

int tga_is_rle(const tga_image *tga)
{
	return (
		tga->image_type == TGA_IMAGE_TYPE_COLORMAP_RLE ||
		tga->image_type == TGA_IMAGE_TYPE_BGR_RLE ||
		tga->image_type == TGA_IMAGE_TYPE_MONO_RLE
		);
}

int tga_is_mono(const tga_image *tga)
{
	return (
		tga->image_type == TGA_IMAGE_TYPE_MONO ||
		tga->image_type == TGA_IMAGE_TYPE_MONO_RLE
		);
}



/* ---------------------------------------------------------------------------
* Convert the numerical <errcode> into a verbose error string.
*
* Returns: an error string
*/
const char *tga_error(const tga_result errcode)
{
	switch (errcode)
	{
	case TGA_NOERR:
		return "no error";
	case TGAERR_FOPEN:
		return "error opening file";
	case TGAERR_EOF:
		return "premature end of file";
	case TGAERR_WRITE:
		return "error writing to file";
	case TGAERR_CMAP_TYPE:
		return "invalid color map type";
	case TGAERR_IMG_TYPE:
		return "invalid image type";
	case TGAERR_NO_IMG:
		return "no image data included";
	case TGAERR_CMAP_MISSING:
		return "color-mapped image without color map";
	case TGAERR_CMAP_PRESENT:
		return "non-color-mapped image with extraneous color map";
	case TGAERR_CMAP_LENGTH:
		return "color map has zero length";
	case TGAERR_CMAP_DEPTH:
		return "invalid color map depth";
	case TGAERR_ZERO_SIZE:
		return "the image dimensions are zero";
	case TGAERR_PIXEL_DEPTH:
		return "invalid pixel depth";
	case TGAERR_NO_MEM:
		return "out of memory";
	case TGAERR_NOT_CMAP:
		return "image is not color mapped";
	case TGAERR_RLE:
		return "RLE data is corrupt";
	case TGAERR_INDEX_RANGE:
		return "color map index out of range";
	case TGAERR_MONO:
		return "image is mono";
	default:
		return "unknown error code";
	}
}



/* ---------------------------------------------------------------------------
* Read a Targa image from a file named <filename> to <dest>.  This is just a
* wrapper around tga_read_from_FILE().
*
* Returns: TGA_NOERR on success, or a matching TGAERR_* code on failure.
*/
tga_result tga_read(tga_image *dest, const char *filename)
{
	tga_result result;
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL) return TGAERR_FOPEN;
	result = tga_read_from_FILE(dest, fp);
	fclose(fp);
	return result;
}



/* ---------------------------------------------------------------------------
* Read a Targa image from <fp> to <dest>.
*
* Returns: TGA_NOERR on success, or a TGAERR_* code on failure.  In the
*          case of failure, the contents of dest are not guaranteed to be
*          valid.
*/
tga_result tga_read_from_FILE(tga_image *dest, FILE *fp)
{
#define BARF(errcode) \
        { tga_free_buffers(dest);  return errcode; }

#define READ(destptr, size) \
        if (fread(destptr, size, 1, fp) != 1) BARF(TGAERR_EOF)

#define READ16(dest) \
        { if (fread(&(dest), 2, 1, fp) != 1) BARF(TGAERR_EOF); \
          dest = letoh16(dest); }

	dest->image_id = NULL;
	dest->color_map_data = NULL;
	dest->image_data = NULL;

	READ(&dest->image_id_length, 1);
	READ(&dest->color_map_type, 1);
	if (dest->color_map_type != TGA_COLOR_MAP_ABSENT &&
		dest->color_map_type != TGA_COLOR_MAP_PRESENT)
		BARF(TGAERR_CMAP_TYPE);

	READ(&dest->image_type, 1);
	if (dest->image_type == TGA_IMAGE_TYPE_NONE)
		BARF(TGAERR_NO_IMG);

	if (dest->image_type != TGA_IMAGE_TYPE_COLORMAP &&
		dest->image_type != TGA_IMAGE_TYPE_BGR &&
		dest->image_type != TGA_IMAGE_TYPE_MONO &&
		dest->image_type != TGA_IMAGE_TYPE_COLORMAP_RLE &&
		dest->image_type != TGA_IMAGE_TYPE_BGR_RLE &&
		dest->image_type != TGA_IMAGE_TYPE_MONO_RLE)
		BARF(TGAERR_IMG_TYPE);

	if (tga_is_colormapped(dest) &&
		dest->color_map_type == TGA_COLOR_MAP_ABSENT)
		BARF(TGAERR_CMAP_MISSING);

	if (!tga_is_colormapped(dest) &&
		dest->color_map_type == TGA_COLOR_MAP_PRESENT)
		BARF(TGAERR_CMAP_PRESENT);

	READ16(dest->color_map_origin);
	READ16(dest->color_map_length);
	READ(&dest->color_map_depth, 1);
	if (dest->color_map_type == TGA_COLOR_MAP_PRESENT)
	{
		if (dest->color_map_length == 0)
			BARF(TGAERR_CMAP_LENGTH);

		if (!UNMAP_DEPTH(dest->color_map_depth))
			BARF(TGAERR_CMAP_DEPTH);
	}

	READ16(dest->origin_x);
	READ16(dest->origin_y);
	READ16(dest->width);
	READ16(dest->height);

	if (dest->width == 0 || dest->height == 0)
		BARF(TGAERR_ZERO_SIZE);

	READ(&dest->pixel_depth, 1);
	if (!SANE_DEPTH(dest->pixel_depth) ||
		(dest->pixel_depth != 8 && tga_is_colormapped(dest)))
		BARF(TGAERR_PIXEL_DEPTH);

	READ(&dest->image_descriptor, 1);

	if (dest->image_id_length > 0)
	{
		dest->image_id = (uint8_t*)malloc(dest->image_id_length);
		if (dest->image_id == NULL) BARF(TGAERR_NO_MEM);
		READ(dest->image_id, dest->image_id_length);
	}

	if (dest->color_map_type == TGA_COLOR_MAP_PRESENT)
	{
		dest->color_map_data = (uint8_t*)malloc(
			(dest->color_map_origin + dest->color_map_length) *
			dest->color_map_depth / 8);
		if (dest->color_map_data == NULL) BARF(TGAERR_NO_MEM);
		READ(dest->color_map_data +
			(dest->color_map_origin * dest->color_map_depth / 8),
			dest->color_map_length * dest->color_map_depth / 8);
	}

	dest->image_data = (uint8_t*)malloc(
		dest->width * dest->height * dest->pixel_depth / 8);
	if (dest->image_data == NULL)
		BARF(TGAERR_NO_MEM);

	if (tga_is_rle(dest))
	{
		/* read RLE */
		tga_result result = tga_read_rle(dest, fp);
		if (result != TGA_NOERR) BARF(result);
	}
	else
	{
		/* uncompressed */
		READ(dest->image_data,
			dest->width * dest->height * dest->pixel_depth / 8);
	}

	return TGA_NOERR;
#undef BARF
#undef READ
#undef READ16
}



/* ---------------------------------------------------------------------------
* Helper function for tga_read_from_FILE().  Decompresses RLE image data from
* <fp>.  Assumes <dest> header fields are set correctly.
*/
static tga_result tga_read_rle(tga_image *dest, FILE *fp)
{
#define RLE_BIT BIT(7)
#define READ(dest, size) \
        if (fread(dest, size, 1, fp) != 1) return TGAERR_EOF

	uint8_t *pos;
	uint32_t p_loaded = 0,
		p_expected = dest->width * dest->height;
	uint8_t bpp = dest->pixel_depth / 8; /* bytes per pixel */

	pos = dest->image_data;

	while ((p_loaded < p_expected) && !feof(fp))
	{
		uint8_t b;
		READ(&b, 1);
		if (b & RLE_BIT)
		{
			/* is an RLE packet */
			uint8_t count, tmp[4], i;

			count = (b & ~RLE_BIT) + 1;
			READ(tmp, bpp);

			for (i = 0; i<count; i++)
			{
				p_loaded++;
				if (p_loaded > p_expected) return TGAERR_RLE;
				memcpy(pos, tmp, bpp);
				pos += bpp;
			}
		}
		else /* RAW packet */
		{
			uint8_t count;

			count = (b & ~RLE_BIT) + 1;
			if (p_loaded + count > p_expected) return TGAERR_RLE;

			p_loaded += count;
			READ(pos, bpp*count);
			pos += count * bpp;
		}
	}
	return TGA_NOERR;
#undef RLE_BIT
#undef READ
}



/* ---------------------------------------------------------------------------
* Write a Targa image to a file named <filename> from <src>.  This is just a
* wrapper around tga_write_to_FILE().
*
* Returns: TGA_NOERR on success, or a matching TGAERR_* code on failure.
*/
tga_result tga_write(const char *filename, const tga_image *src)
{
	tga_result result;
	FILE *fp = fopen(filename, "wb");
	if (fp == NULL) return TGAERR_FOPEN;
	result = tga_write_to_FILE(fp, src);
	fclose(fp);
	return result;
}



/* ---------------------------------------------------------------------------
* Write one row of an image to <fp> using RLE.  This is a helper function
* called from tga_write_to_FILE().  It assumes that <src> has its header
* fields set up correctly.
*/
#define PIXEL(ofs) ( row + (ofs)*bpp )
static tga_result tga_write_row_RLE(FILE *fp,
	const tga_image *src, const uint8_t *row)
{
#define WRITE(src, size) \
        if (fwrite(src, size, 1, fp) != 1) return TGAERR_WRITE

	uint16_t pos = 0;
	uint16_t bpp = src->pixel_depth / 8;

	while (pos < src->width)
	{
		packet_type type = rle_packet_type(row, pos, src->width, bpp);
		uint8_t len = rle_packet_len(row, pos, src->width, bpp, type);
		uint8_t packet_header;

		packet_header = len - 1;
		if (type == RLE) packet_header |= BIT(7);

		WRITE(&packet_header, 1);
		if (type == RLE)
		{
			WRITE(PIXEL(pos), bpp);
		}
		else /* type == RAW */
		{
			WRITE(PIXEL(pos), bpp*len);
		}

		pos += len;
	}

	return TGA_NOERR;
#undef WRITE
}



/* ---------------------------------------------------------------------------
* Determine whether the next packet should be RAW or RLE for maximum
* efficiency.  This is a helper function called from rle_packet_len() and
* tga_write_row_RLE().
*/
#define SAME(ofs1, ofs2) (memcmp(PIXEL(ofs1), PIXEL(ofs2), bpp) == 0)

static packet_type rle_packet_type(const uint8_t *row, const uint16_t pos,
	const uint16_t width, const uint16_t bpp)
{
	if (pos == width - 1) return RAW; /* one pixel */
	if (SAME(pos, pos + 1)) /* dupe pixel */
	{
		if (bpp > 1) return RLE; /* inefficient for bpp=1 */

								 /* three repeats makes the bpp=1 case efficient enough */
		if ((pos < width - 2) && SAME(pos + 1, pos + 2)) return RLE;
	}
	return RAW;
}



/* ---------------------------------------------------------------------------
* Find the length of the current RLE packet.  This is a helper function
* called from tga_write_row_RLE().
*/
static uint8_t rle_packet_len(const uint8_t *row, const uint16_t pos,
	const uint16_t width, const uint16_t bpp, const packet_type type)
{
	uint8_t len = 2;

	if (pos == width - 1) return 1;
	if (pos == width - 2) return 2;

	if (type == RLE)
	{
		while (pos + len < width)
		{
			if (SAME(pos, pos + len))
				len++;
			else
				return len;

			if (len == 128) return 128;
		}
	}
	else /* type == RAW */
	{
		while (pos + len < width)
		{
			if (rle_packet_type(row, pos + len, width, bpp) == RAW)
				len++;
			else
				return len;
			if (len == 128) return 128;
		}
	}
	return len; /* hit end of row (width) */
}
#undef SAME
#undef PIXEL



/* ---------------------------------------------------------------------------
* Writes a Targa image to <fp> from <src>.
*
* Returns: TGA_NOERR on success, or a TGAERR_* code on failure.
*          On failure, the contents of the file are not guaranteed
*          to be valid.
*/
tga_result tga_write_to_FILE(FILE *fp, const tga_image *src)
{
#define WRITE(srcptr, size) \
        if (fwrite(srcptr, size, 1, fp) != 1) return TGAERR_WRITE

#define WRITE16(src) \
        { uint16_t _temp = htole16(src); \
          if (fwrite(&_temp, 2, 1, fp) != 1) return TGAERR_WRITE; }

	WRITE(&src->image_id_length, 1);

	if (src->color_map_type != TGA_COLOR_MAP_ABSENT &&
		src->color_map_type != TGA_COLOR_MAP_PRESENT)
		return TGAERR_CMAP_TYPE;
	WRITE(&src->color_map_type, 1);

	if (src->image_type == TGA_IMAGE_TYPE_NONE)
		return TGAERR_NO_IMG;
	if (src->image_type != TGA_IMAGE_TYPE_COLORMAP &&
		src->image_type != TGA_IMAGE_TYPE_BGR &&
		src->image_type != TGA_IMAGE_TYPE_MONO &&
		src->image_type != TGA_IMAGE_TYPE_COLORMAP_RLE &&
		src->image_type != TGA_IMAGE_TYPE_BGR_RLE &&
		src->image_type != TGA_IMAGE_TYPE_MONO_RLE)
		return TGAERR_IMG_TYPE;
	WRITE(&src->image_type, 1);

	if (tga_is_colormapped(src) &&
		src->color_map_type == TGA_COLOR_MAP_ABSENT)
		return TGAERR_CMAP_MISSING;
	if (!tga_is_colormapped(src) &&
		src->color_map_type == TGA_COLOR_MAP_PRESENT)
		return TGAERR_CMAP_PRESENT;
	if (src->color_map_type == TGA_COLOR_MAP_PRESENT)
	{
		if (src->color_map_length == 0)
			return TGAERR_CMAP_LENGTH;

		if (!UNMAP_DEPTH(src->color_map_depth))
			return TGAERR_CMAP_DEPTH;
	}
	WRITE16(src->color_map_origin);
	WRITE16(src->color_map_length);
	WRITE(&src->color_map_depth, 1);

	WRITE16(src->origin_x);
	WRITE16(src->origin_y);

	if (src->width == 0 || src->height == 0)
		return TGAERR_ZERO_SIZE;
	WRITE16(src->width);
	WRITE16(src->height);

	if (!SANE_DEPTH(src->pixel_depth) ||
		(src->pixel_depth != 8 && tga_is_colormapped(src)))
		return TGAERR_PIXEL_DEPTH;
	WRITE(&src->pixel_depth, 1);

	WRITE(&src->image_descriptor, 1);

	if (src->image_id_length > 0)
		WRITE(&src->image_id, src->image_id_length);

	if (src->color_map_type == TGA_COLOR_MAP_PRESENT)
		WRITE(src->color_map_data +
		(src->color_map_origin * src->color_map_depth / 8),
			src->color_map_length * src->color_map_depth / 8);

	if (tga_is_rle(src))
	{
		uint16_t row;
		for (row = 0; row<src->height; row++)
		{
			tga_result result = tga_write_row_RLE(fp, src,
				src->image_data + row * src->width*src->pixel_depth / 8);
			if (result != TGA_NOERR) return result;
		}
	}
	else
	{
		/* uncompressed */
		WRITE(src->image_data,
			src->width * src->height * src->pixel_depth / 8);
	}

	WRITE(tga_id, tga_id_length);

	return TGA_NOERR;
#undef WRITE
#undef WRITE16
}



/* Convenient writing functions --------------------------------------------*/

/*
* This is just a helper function to initialise the header fields in a
* tga_image struct.
*/
static void init_tga_image(tga_image *img, uint8_t *image,
	const uint16_t width, const uint16_t height, const uint8_t depth)
{
	img->image_id_length = 0;
	img->color_map_type = TGA_COLOR_MAP_ABSENT;
	img->image_type = TGA_IMAGE_TYPE_NONE; /* override this below! */
	img->color_map_origin = 0;
	img->color_map_length = 0;
	img->color_map_depth = 0;
	img->origin_x = 0;
	img->origin_y = 0;
	img->width = width;
	img->height = height;
	img->pixel_depth = depth;
	img->image_descriptor = TGA_T_TO_B_BIT;
	img->image_id = NULL;
	img->color_map_data = NULL;
	img->image_data = image;
}



tga_result tga_write_mono(const char *filename, uint8_t *image,
	const uint16_t width, const uint16_t height)
{
	tga_image img;
	init_tga_image(&img, image, width, height, 8);
	img.image_type = TGA_IMAGE_TYPE_MONO;
	return tga_write(filename, &img);
}



tga_result tga_write_mono_rle(const char *filename, uint8_t *image,
	const uint16_t width, const uint16_t height)
{
	tga_image img;
	init_tga_image(&img, image, width, height, 8);
	img.image_type = TGA_IMAGE_TYPE_MONO_RLE;
	return tga_write(filename, &img);
}



tga_result tga_write_bgr(const char *filename, uint8_t *image,
	const uint16_t width, const uint16_t height, const uint8_t depth)
{
	tga_image img;
	init_tga_image(&img, image, width, height, depth);
	img.image_type = TGA_IMAGE_TYPE_BGR;
	return tga_write(filename, &img);
}



tga_result tga_write_bgr_rle(const char *filename, uint8_t *image,
	const uint16_t width, const uint16_t height, const uint8_t depth)
{
	tga_image img;
	init_tga_image(&img, image, width, height, depth);
	img.image_type = TGA_IMAGE_TYPE_BGR_RLE;
	return tga_write(filename, &img);
}



/* Note: this function will MODIFY <image> */
tga_result tga_write_rgb(const char *filename, uint8_t *image,
	const uint16_t width, const uint16_t height, const uint8_t depth)
{
	tga_image img;
	init_tga_image(&img, image, width, height, depth);
	img.image_type = TGA_IMAGE_TYPE_BGR;
	(void)tga_swap_red_blue(&img);
	return tga_write(filename, &img);
}



/* Note: this function will MODIFY <image> */
tga_result tga_write_rgb_rle(const char *filename, uint8_t *image,
	const uint16_t width, const uint16_t height, const uint8_t depth)
{
	tga_image img;
	init_tga_image(&img, image, width, height, depth);
	img.image_type = TGA_IMAGE_TYPE_BGR_RLE;
	(void)tga_swap_red_blue(&img);
	return tga_write(filename, &img);
}






/* ---------------------------------------------------------------------------
* Free the image_id, color_map_data and image_data buffers of the specified
* tga_image, if they're not already NULL.
*/
void tga_free_buffers(tga_image *img)
{
	if (img->image_id != NULL)
	{
		free(img->image_id);
		img->image_id = NULL;
	}
	if (img->color_map_data != NULL)
	{
		free(img->color_map_data);
		img->color_map_data = NULL;
	}
	if (img->image_data != NULL)
	{
		free(img->image_data);
		img->image_data = NULL;
	}
}

/* vim:set tabstop=4 shiftwidth=4 textwidth=78 expandtab: */


