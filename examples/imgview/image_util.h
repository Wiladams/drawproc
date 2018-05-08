#pragma once
/* Manipulation ------------------------------------------------------------*/
tga_result tga_flip_horiz(tga_image *img);
tga_result tga_flip_vert(tga_image *img);
tga_result tga_color_unmap(tga_image *img);

uint8_t *tga_find_pixel(const tga_image *img, uint16_t x, uint16_t y);
tga_result tga_unpack_pixel(const uint8_t *src, const uint8_t bits,
	uint8_t *b, uint8_t *g, uint8_t *r, uint8_t *a);
tga_result tga_pack_pixel(uint8_t *dest, const uint8_t bits,
	const uint8_t b, const uint8_t g, const uint8_t r, const uint8_t a);

tga_result tga_desaturate(tga_image *img,
	const int cr, const int cg, const int cb, const int dv);
tga_result tga_desaturate_rec_601_1(tga_image *img);
tga_result tga_desaturate_rec_709(tga_image *img);
tga_result tga_desaturate_itu(tga_image *img);
tga_result tga_desaturate_avg(tga_image *img);
tga_result tga_convert_depth(tga_image *img, const uint8_t bits);
tga_result tga_swap_red_blue(tga_image *img);