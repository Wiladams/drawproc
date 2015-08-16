/*
Copyright 2015 William A Adams

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/


#include "graphicc.h"

void font_t_init(font_t *f, const uint8_t *data)
{
	f->bigendian = isBigEndian();
	f->height = data[0];
	f->baseline = data[1];
	f->start_char = data[2];
	f->num_chars = data[3];
	f->charbits = (uint8_t *)(data + 4);
}

// Create a 16-bit value, taking into account
// the endianness of the host.
// The data is in little endian format
uint16_t font_t_value(const font_t *f, const uint8_t* p)
{
	uint16_t v;
	if (!f->bigendian)
	{
		v = p[0] + (p[1] << 8);
		//*(uint8_t*)&v = p[0];
		//*((uint8_t*)&v + 1) = p[1];
	}
	else
	{
		v = p[1] + (p[0] << 8);
		//*(uint8_t*)&v = p[1];
		//*((uint8_t*)&v + 1) = p[0];
	}

	return v;
}

uint8_t * font_t_glyph_pointer(const font_t *f, const unsigned int glyph)
{
	return (f)->charbits + (f)->num_chars * 2 + font_t_value(f, (f)->charbits + (glyph - (f)->start_char) * 2);
}

// Prepare a glyph to be written to a specific
// position
void glyph_t_prepare(const font_t *font, const glyph_t *ginfo, struct glyph_rect* r, double x, double y, bool flip)
{
	r->x1 = (int)x;
	r->x2 = r->x1 + ginfo->width - 1;
	if (flip)
	{
		r->y1 = int(y) - font->height + font->baseline;
		r->y2 = r->y1 + font->height - 1;
	}
	else
	{
		r->y1 = int(y) - font->baseline + 1;
		r->y2 = r->y1 + font->height - 1;
	}
	r->dx = ginfo->width;
	r->dy = 0;
}

// Fill in the meta information for the specified glyph
void glyph_t_init(const font_t *f, glyph_t *ginfo, const unsigned int glyph)
{
	uint8_t * glyphptr = font_t_glyph_pointer(f, glyph);

	ginfo->width = glyphptr[0];
	ginfo->data = glyphptr + 1;
	ginfo->byte_width = (ginfo->width + 7) >> 3;
}

size_t font_t_glyph_width(const font_t *f, const unsigned int glyph)
{
	glyph_t ginfo;
	glyph_t_init(f, &ginfo, glyph);

	return ginfo.width;
}

// Figure out the width of a string in a given font
size_t font_t_str_width(const font_t *f, const char * str)
{
	size_t w = 0;

	while (*str)
	{
		w += font_t_glyph_width(f, *str);
		++str;
	}

	return w;
}



// Create a single scanline of the glyph
void glyph_t_span(const font_t *f, glyph_t *g, unsigned i, cover_type *m_span)
{
	i = f->height - i - 1;

	const uint8_t *bits = g->data + i * g->byte_width;
	unsigned int j;
	unsigned int val = *bits;
	unsigned int nb = 0;

	for (j = 0; j < g->width; ++j)
	{
		m_span[j] = (cover_type)((val & 0x80) ? cover_full : cover_none);
		val <<= 1;
		if (++nb >= 8)
		{
			val = *++bits;
			nb = 0;
		}
	}

	//return m_span;
}

int scan_glyph(pb_rgba *pb, font_t *font, glyph_t *glyph, const int x, const int y, const uint32_t color)
{
	cover_type m_span[32];
	int line = font->height;

	while (line--)
	{
		glyph_t_span(font, glyph, line, m_span);

		// transfer the span to the bitmap
		int spanwidth = glyph->width;
		while (spanwidth--) {
			if (m_span[spanwidth] == cover_full) {
				pb_rgba_cover_pixel(pb, x + spanwidth, y + font->height - line, color);
			}
		}
		memset(m_span, 0, 32);
	}

	return glyph->width;

}

int scan_str(pb_rgba *pb, font_t *font, const int x, const int y, const char *chars, const uint32_t color)
{
	glyph_t ginfo;

	int idx = 0;
	int dx = x;
	int dy = y;

	while (chars[idx])
	{
		glyph_t_init(font, &ginfo, chars[idx]);
		scan_glyph(pb, font, &ginfo, dx, dy, color);
		dx += ginfo.width;
		idx++;
	}

	return dx;
}
