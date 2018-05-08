#pragma once
/* ---------------------------------------------------------------------------
* Truevision Targa Reader/Writer
* $Id: targa.h,v 1.8 2004/10/09 09:30:26 emikulic Exp $
*
* Copyright (C) 2001-2003 Emil Mikulic.
*
* Source and binary redistribution of this code, with or without
* changes, for free or for profit, is allowed as long as this copyright
* notice is kept intact.  Modified versions have to be clearly marked
* as modified.
*
* This code is provided without any warranty.  The copyright holder is
* not liable for anything bad that might happen as a result of the
* code.
* -------------------------------------------------------------------------*/

#include <stdio.h>
#include <inttypes.h>

#define BIT(index) (1 << (index))

#ifdef _BIG_ENDIAN
# define htole16(x) ( (((x) & 0x00FF) << 8) | (((x) & 0xFF00) >> 8) )
# define letoh16(x) htole16(x)
#else /* little endian */
# define htole16(x) (x)
# define letoh16(x) (x)
#endif /* endianness */

// color_map_type
#define TGA_COLOR_MAP_ABSENT    0
#define TGA_COLOR_MAP_PRESENT   1

// image_type
#define TGA_IMAGE_TYPE_NONE          0 /* no image data */
#define TGA_IMAGE_TYPE_COLORMAP      1 /* uncompressed, color-mapped */
#define TGA_IMAGE_TYPE_BGR           2 /* uncompressed, true-color */
#define TGA_IMAGE_TYPE_MONO          3 /* uncompressed, black and white */
#define TGA_IMAGE_TYPE_COLORMAP_RLE  9 /* run-length, color-mapped */
#define TGA_IMAGE_TYPE_BGR_RLE      10 /* run-length, true-color */
#define TGA_IMAGE_TYPE_MONO_RLE     11 /* run-length, black and white */

// image_descriptor
/* bits 0,1,2,3 - attribute bits per pixel
* bit  4       - set if image is stored right-to-left
* bit  5       - set if image is stored top-to-bottom
* bits 6,7     - unused (must be set to zero)
*/

#define TGA_ATTRIB_BITS (uint8_t)(BIT(0)|BIT(1)|BIT(2)|BIT(3))
#define TGA_R_TO_L_BIT  (uint8_t)BIT(4)
#define TGA_T_TO_B_BIT  (uint8_t)BIT(5)
#define TGA_UNUSED_BITS (uint8_t)(BIT(6)|BIT(7))
/* Note: right-to-left order is not honored by some Targa readers */

// image_id
/* The length of this field is given in image_id_length, it's read raw
* from the file so it's not not guaranteed to be zero-terminated.  If
* it's not NULL, it needs to be deallocated.  see: tga_free_buffers()
*/



/* Targa image and header fields -------------------------------------------*/
typedef struct
{
	// Note that Targa is stored in little-endian order
	uint8_t     image_id_length;
	uint8_t     color_map_type;

	uint8_t     image_type;

	// color map specification 
	uint16_t    color_map_origin;   // index of first entry 
	uint16_t    color_map_length;   // number of entries included 
	uint8_t     color_map_depth;    // number of bits per entry 

	// image specification	
	uint16_t    origin_x;
	uint16_t    origin_y;
	uint16_t    width;
	uint16_t    height;
	uint8_t     pixel_depth;

	uint8_t  image_descriptor;
	uint8_t *image_id;
	uint8_t *color_map_data;
	uint8_t *image_data;
} tga_image;



/* For decoding header bits ------------------------------------------------*/
uint8_t tga_get_attribute_bits(const tga_image *tga);
int tga_is_right_to_left(const tga_image *tga);
int tga_is_top_to_bottom(const tga_image *tga);
int tga_is_colormapped(const tga_image *tga);
int tga_is_rle(const tga_image *tga);
int tga_is_mono(const tga_image *tga);



/* Error handling ----------------------------------------------------------*/
typedef enum {
	TGA_NOERR,
	TGAERR_FOPEN,
	TGAERR_EOF,
	TGAERR_WRITE,
	TGAERR_CMAP_TYPE,
	TGAERR_IMG_TYPE,
	TGAERR_NO_IMG,
	TGAERR_CMAP_MISSING,
	TGAERR_CMAP_PRESENT,
	TGAERR_CMAP_LENGTH,
	TGAERR_CMAP_DEPTH,
	TGAERR_ZERO_SIZE,
	TGAERR_PIXEL_DEPTH,
	TGAERR_NO_MEM,
	TGAERR_NOT_CMAP,
	TGAERR_RLE,
	TGAERR_INDEX_RANGE,
	TGAERR_MONO
} tga_result;

const char *tga_error(const tga_result errcode);



/* Load/save ---------------------------------------------------------------*/
tga_result tga_read(tga_image *dest, const char *filename);
tga_result tga_read_from_FILE(tga_image *dest, FILE *fp);
tga_result tga_write(const char *filename, const tga_image *src);
tga_result tga_write_to_FILE(FILE *fp, const tga_image *src);



/* Convenient writing functions --------------------------------------------*/
tga_result tga_write_mono(const char *filename, uint8_t *image,
	const uint16_t width, const uint16_t height);

tga_result tga_write_mono_rle(const char *filename, uint8_t *image,
	const uint16_t width, const uint16_t height);

tga_result tga_write_bgr(const char *filename, uint8_t *image,
	const uint16_t width, const uint16_t height, const uint8_t depth);

tga_result tga_write_bgr_rle(const char *filename, uint8_t *image,
	const uint16_t width, const uint16_t height, const uint8_t depth);

/* These functions will use tga_swap_red_blue to MODIFY your image data */
tga_result tga_write_rgb(const char *filename, uint8_t *image,
	const uint16_t width, const uint16_t height, const uint8_t depth);

tga_result tga_write_rgb_rle(const char *filename, uint8_t *image,
	const uint16_t width, const uint16_t height, const uint8_t depth);





void tga_free_buffers(tga_image *img);



#ifndef TGA_KEEP_MACROS /* useful for targa.c */
# undef htole16
# undef letoh16
#endif
