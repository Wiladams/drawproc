/* Convenient manipulation functions ---------------------------------------*/

/* ---------------------------------------------------------------------------
* Horizontally flip the image in place.  Reverses the right-to-left bit in
* the image descriptor.
*/
tga_result tga_flip_horiz(tga_image *img)
{
	uint16_t row;
	size_t bpp;
	uint8_t *left, *right;
	int r_to_l;

	if (!SANE_DEPTH(img->pixel_depth)) return TGAERR_PIXEL_DEPTH;
	bpp = (size_t)(img->pixel_depth / 8); /* bytes per pixel */

	for (row = 0; row<img->height; row++)
	{
		left = img->image_data + row * img->width * bpp;
		right = left + (img->width - 1) * bpp;

		/* reverse from left to right */
		while (left < right)
		{
			uint8_t buffer[4];

			/* swap */
			memcpy(buffer, left, bpp);
			memcpy(left, right, bpp);
			memcpy(right, buffer, bpp);

			left += bpp;
			right -= bpp;
		}
	}

	/* Correct image_descriptor's left-to-right-ness. */
	r_to_l = tga_is_right_to_left(img);
	img->image_descriptor &= ~TGA_R_TO_L_BIT; /* mask out r-to-l bit */
	if (!r_to_l)
		/* was l-to-r, need to set r_to_l */
		img->image_descriptor |= TGA_R_TO_L_BIT;
	/* else bit is already rubbed out */

	return TGA_NOERR;
}



/* ---------------------------------------------------------------------------
* Vertically flip the image in place.  Reverses the top-to-bottom bit in
* the image descriptor.
*/
tga_result tga_flip_vert(tga_image *img)
{
	uint16_t col;
	size_t bpp, line;
	uint8_t *top, *bottom;
	int t_to_b;

	if (!SANE_DEPTH(img->pixel_depth)) return TGAERR_PIXEL_DEPTH;
	bpp = (size_t)(img->pixel_depth / 8);   /* bytes per pixel */
	line = bpp * img->width;                /* bytes per line */

	for (col = 0; col<img->width; col++)
	{
		top = img->image_data + col * bpp;
		bottom = top + (img->height - 1) * line;

		/* reverse from top to bottom */
		while (top < bottom)
		{
			uint8_t buffer[4];

			/* swap */
			memcpy(buffer, top, bpp);
			memcpy(top, bottom, bpp);
			memcpy(bottom, buffer, bpp);

			top += line;
			bottom -= line;
		}
	}

	/* Correct image_descriptor's top-to-bottom-ness. */
	t_to_b = tga_is_top_to_bottom(img);
	img->image_descriptor &= ~TGA_T_TO_B_BIT; /* mask out t-to-b bit */
	if (!t_to_b)
		/* was b-to-t, need to set t_to_b */
		img->image_descriptor |= TGA_T_TO_B_BIT;
	/* else bit is already rubbed out */

	return TGA_NOERR;
}



/* ---------------------------------------------------------------------------
* Convert a color-mapped image to unmapped BGR.  Reallocates image_data to a
* bigger size, then converts the image backwards to avoid using a secondary
* buffer.  Alters the necessary header fields and deallocates the color map.
*/
tga_result tga_color_unmap(tga_image *img)
{
	uint8_t bpp = img->color_map_depth / 8; /* bytes per pixel */
	int pos;
	void *tmp;

	if (!tga_is_colormapped(img)) return TGAERR_NOT_CMAP;
	if (img->pixel_depth != 8) return TGAERR_PIXEL_DEPTH;
	if (!SANE_DEPTH(img->color_map_depth)) return TGAERR_CMAP_DEPTH;

	tmp = realloc(img->image_data, img->width * img->height * bpp);
	if (tmp == NULL) return TGAERR_NO_MEM;
	img->image_data = (uint8_t*)tmp;

	for (pos = img->width * img->height - 1; pos >= 0; pos--)
	{
		uint8_t c_index = img->image_data[pos];
		uint8_t *c_bgr = img->color_map_data + (c_index * bpp);

		if (c_index >= img->color_map_origin + img->color_map_length)
			return TGAERR_INDEX_RANGE;

		memcpy(img->image_data + (pos*bpp), c_bgr, (size_t)bpp);
	}

	/* clean up */
	img->image_type = TGA_IMAGE_TYPE_BGR;
	img->pixel_depth = img->color_map_depth;

	free(img->color_map_data);
	img->color_map_data = NULL;
	img->color_map_type = TGA_COLOR_MAP_ABSENT;
	img->color_map_origin = 0;
	img->color_map_length = 0;
	img->color_map_depth = 0;

	return TGA_NOERR;
}



/* ---------------------------------------------------------------------------
* Return a pointer to a given pixel.  Accounts for image orientation (T_TO_B,
* R_TO_L, etc).  Returns NULL if the pixel is out of range.
*/
uint8_t *tga_find_pixel(const tga_image *img, uint16_t x, uint16_t y)
{
	if (x >= img->width || y >= img->height)
		return NULL;

	if (!tga_is_top_to_bottom(img)) y = img->height - 1 - y;
	if (tga_is_right_to_left(img)) x = img->width - 1 - x;
	return img->image_data + (x + y * img->width) * img->pixel_depth / 8;
}



/* ---------------------------------------------------------------------------
* Unpack the pixel at the src pointer according to bits.  Any of b,g,r,a can
* be set to NULL if not wanted.  Returns TGAERR_PIXEL_DEPTH if a stupid
* number of bits is given.
*/
tga_result tga_unpack_pixel(const uint8_t *src, const uint8_t bits,
	uint8_t *b, uint8_t *g, uint8_t *r, uint8_t *a)
{
	switch (bits)
	{
	case 32:
		if (b) *b = src[0];
		if (g) *g = src[1];
		if (r) *r = src[2];
		if (a) *a = src[3];
		break;

	case 24:
		if (b) *b = src[0];
		if (g) *g = src[1];
		if (r) *r = src[2];
		if (a) *a = 0;
		break;

	case 16:
	{
		uint16_t src16 = (uint16_t)(src[1] << 8) | (uint16_t)src[0];

#define FIVE_BITS (BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4))
		if (b) *b = ((src16)& FIVE_BITS) << 3;
		if (g) *g = ((src16 >> 5) & FIVE_BITS) << 3;
		if (r) *r = ((src16 >> 10) & FIVE_BITS) << 3;
		if (a) *a = (uint8_t)((src16 & BIT(15)) ? 255 : 0);
#undef FIVE_BITS
		break;
	}

	case 8:
		if (b) *b = *src;
		if (g) *g = *src;
		if (r) *r = *src;
		if (a) *a = 0;
		break;

	default:
		return TGAERR_PIXEL_DEPTH;
	}
	return TGA_NOERR;
}



/* ---------------------------------------------------------------------------
* Pack the pixel at the dest pointer according to bits.  Returns
* TGAERR_PIXEL_DEPTH if a stupid number of bits is given.
*/
tga_result tga_pack_pixel(uint8_t *dest, const uint8_t bits,
	const uint8_t b, const uint8_t g, const uint8_t r, const uint8_t a)
{
	switch (bits)
	{
	case 32:
		dest[0] = b;
		dest[1] = g;
		dest[2] = r;
		dest[3] = a;
		break;

	case 24:
		dest[0] = b;
		dest[1] = g;
		dest[2] = r;
		break;

	case 16:
	{
		uint16_t tmp;

#define FIVE_BITS (BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4))
		tmp = (b >> 3) & FIVE_BITS;
		tmp |= ((g >> 3) & FIVE_BITS) << 5;
		tmp |= ((r >> 3) & FIVE_BITS) << 10;
		if (a > 127) tmp |= BIT(15);
#undef FIVE_BITS

		dest[0] = (uint8_t)(tmp & 0x00FF);
		dest[1] = (uint8_t)((tmp & 0xFF00) >> 8);
		break;
	}

	default:
		return TGAERR_PIXEL_DEPTH;
	}
	return TGA_NOERR;
}



/* ---------------------------------------------------------------------------
* Desaturate the specified Targa using the specified coefficients:
*      output = ( red * cr + green * cg + blue * cb ) / dv
*/
tga_result tga_desaturate(tga_image *img, const int cr, const int cg,
	const int cb, const int dv)
{
	uint8_t bpp = img->pixel_depth / 8; /* bytes per pixel */
	uint8_t *dest, *src, *tmp;

	if (tga_is_mono(img)) return TGAERR_MONO;
	if (tga_is_colormapped(img))
	{
		tga_result result = tga_color_unmap(img);
		if (result != TGA_NOERR) return result;
	}
	if (!UNMAP_DEPTH(img->pixel_depth)) return TGAERR_PIXEL_DEPTH;

	dest = img->image_data;
	for (src = img->image_data;
		src < img->image_data + img->width*img->height*bpp;
		src += bpp)
	{
		uint8_t b, g, r;
		(void)tga_unpack_pixel(src, img->pixel_depth, &b, &g, &r, NULL);

		*dest = (uint8_t)(((int)b * cb +
			(int)g * cg +
			(int)r * cr) / dv);
		dest++;
	}

	/* shrink */
	tmp = realloc(img->image_data, img->width * img->height);
	if (tmp == NULL) return TGAERR_NO_MEM;
	img->image_data = tmp;

	img->pixel_depth = 8;
	img->image_type = TGA_IMAGE_TYPE_MONO;
	return TGA_NOERR;
}

tga_result tga_desaturate_rec_601_1(tga_image *img)
{
	return tga_desaturate(img, 2989, 5866, 1145, 10000);
}

tga_result tga_desaturate_rec_709(tga_image *img)
{
	return tga_desaturate(img, 2126, 7152, 722, 10000);
}

tga_result tga_desaturate_itu(tga_image *img)
{
	return tga_desaturate(img, 2220, 7067, 713, 10000);
}

tga_result tga_desaturate_avg(tga_image *img)
{
	return tga_desaturate(img, 1, 1, 1, 3);
}



/* ---------------------------------------------------------------------------
* Convert an image to the given pixel depth. (one of 32, 24, 16)  Avoids
* using a secondary buffer to do the conversion.
*/
tga_result tga_convert_depth(tga_image *img, const uint8_t bits)
{
	size_t src_size, dest_size;
	uint8_t src_bpp, dest_bpp;
	uint8_t *src, *dest;

	if (!UNMAP_DEPTH(bits) ||
		!SANE_DEPTH(img->pixel_depth)
		)    return TGAERR_PIXEL_DEPTH;

	if (tga_is_colormapped(img))
	{
		tga_result result = tga_color_unmap(img);
		if (result != TGA_NOERR) return result;
	}

	if (img->pixel_depth == bits) return TGA_NOERR; /* no op, no err */

	src_bpp = img->pixel_depth / 8;
	dest_bpp = bits / 8;

	src_size = (size_t)(img->width * img->height * src_bpp);
	dest_size = (size_t)(img->width * img->height * dest_bpp);

	if (src_size > dest_size)
	{
		void *tmp;

		/* convert forwards */
		dest = img->image_data;
		for (src = img->image_data;
			src < img->image_data + img->width * img->height * src_bpp;
			src += src_bpp)
		{
			uint8_t r, g, b, a;
			(void)tga_unpack_pixel(src, img->pixel_depth, &r, &g, &b, &a);
			(void)tga_pack_pixel(dest, bits, r, g, b, a);
			dest += dest_bpp;
		}

		/* shrink */
		tmp = realloc(img->image_data, img->width * img->height * dest_bpp);
		if (tmp == NULL) return TGAERR_NO_MEM;
		img->image_data = tmp;
	}
	else
	{
		/* expand */
		void *tmp = realloc(img->image_data,
			img->width * img->height * dest_bpp);
		if (tmp == NULL) return TGAERR_NO_MEM;
		img->image_data = (uint8_t*)tmp;

		/* convert backwards */
		dest = img->image_data + (img->width*img->height - 1) * dest_bpp;
		for (src = img->image_data + (img->width*img->height - 1) * src_bpp;
			src >= img->image_data;
			src -= src_bpp)
		{
			uint8_t r, g, b, a;
			(void)tga_unpack_pixel(src, img->pixel_depth, &r, &g, &b, &a);
			(void)tga_pack_pixel(dest, bits, r, g, b, a);
			dest -= dest_bpp;
		}
	}

	img->pixel_depth = bits;
	return TGA_NOERR;
}



/* ---------------------------------------------------------------------------
* Swap red and blue (RGB becomes BGR and vice verse).  (in-place)
*/
tga_result tga_swap_red_blue(tga_image *img)
{
	uint8_t *ptr;
	uint8_t bpp = img->pixel_depth / 8;

	if (!UNMAP_DEPTH(img->pixel_depth)) return TGAERR_PIXEL_DEPTH;

	for (ptr = img->image_data;
		ptr < img->image_data + (img->width * img->height - 1) * bpp;
		ptr += bpp)
	{
		uint8_t r, g, b, a;
		(void)tga_unpack_pixel(ptr, img->pixel_depth, &b, &g, &r, &a);
		(void)tga_pack_pixel(ptr, img->pixel_depth, r, g, b, a);
	}
	return TGA_NOERR;
}
