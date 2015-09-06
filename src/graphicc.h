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

#pragma once

#ifndef graphicc_h
#define graphicc_h

#include "dproc_config.h"
#include "dproc_constants.h"



#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define BGR_DOMINANT 1



#include "pb_rect.h"
#include "Point3D.h"



inline bool isBigEndian() {int t = 1;return (*(char*)&t == 0);}

inline real DEGREES(const real radians) { return (real)(G_RTOD * radians); }
inline real RADIANS(const real degrees) { return (real)(G_DTOR * degrees); }

// utils
#define min3(a,b,c) __min(__min(a,b),c)
#define max3(a,b,c) __max(__max(a,b),c)
#define min(a,b) __min(a,b)
#define max(a,b) __max(a,b)


// map a value (a) from between rlo <= a <= rhi to  shi <= b <= slo
inline double MAP(double a, double rlo, double rhi, double slo, double shi) { return slo + (a - rlo) * (shi - slo) / (rhi - rlo); }
inline double CLAMP(double a, double rlo, double rhi){ return a < rlo ? rlo : (a>rhi ? rhi : a); }
//inline double CLAMP(double a, double rlo, double rhi){ return __min(__max(a, rlo), rhi); }
#define map MAP

// turn a division by 255 into something 
// much cheaper to calculate
// for values between 0 and 65534
#define div255(num) ((num + (num >> 8)) >> 8)
#define lerp255(bg, fg, a) ((uint8_t)div255((fg*a+bg*(255-a))))

// returns the sign of the value
//  < 0 --> -1
//  > 0 -->  1
// == 0 -->  0
inline int sgn(real val) { return ((0 < val) - (val < 0)); }


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4201)  // nonstandard extension used : nameless struct/union
#endif


#ifdef BGR_DOMINANT
inline uint32_t RGBA(const int r, const int g, const int b, const int a) 
{
	return ((uint32_t)(a << 24 | r << 16 | g << 8 | b));
}

inline uint8_t GET_B(const uint32_t value) {return ((uint32_t)value & 0xff);}
inline uint8_t GET_G(const uint32_t value) {return (((uint32_t)value & 0xff00) >> 8);}
inline uint8_t GET_R(const uint32_t value) {return (((uint32_t)value & 0xff0000) >> 16);}
inline uint8_t GET_A(const uint32_t value) {return (((uint32_t)value & 0xff000000) >> 24);}
#else
#define RGBA(r,g,b,a) ((uint32_t)(a<<24|b<<16|g<<8|r))
#define GET_R(value) ((uint32_t)value &0xff)
#define GET_G(value) (((uint32_t)value &0xff00) >> 8)
#define GET_B(value) (((uint32_t)value &0xff0000) >> 16)
#define GET_A(value) (((uint32_t)value &0xff000000) >> 24)
#endif

// pixel buffer color
typedef struct {
	union {
		struct {
			uint8_t r, g, b, a;
		};
		uint8_t v_[4];
		uint32_t value;
	};
} pix_rgba;








#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif

/*
A pixel buffer is an array of pixels.

The pixelformat determines the layout of the
individual elements.

data		- pointer to the beginning of the data
bitStride	- number of bits between rows
pixelpitch	- number of pixels between rows
*/

typedef struct _pb_rgba {
	uint8_t *		data;
	unsigned int	pixelpitch;
	int				bitStride;
	int				owndata;
	pb_rect			frame;
} pb_rgba;



#ifdef __cplusplus
extern "C" {
#endif

DPROC_API int pb_rgba_init(pb_rgba *pb, const unsigned int width, const unsigned int height);
DPROC_API int pb_rgba_free(pb_rgba *pb);

DPROC_API int pb_rgba_get_frame(pb_rgba *src, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, pb_rgba *pf);

DPROC_API void pb_rgba_cover_pixel(pb_rgba *pb, const unsigned int x, const unsigned int y, const uint32_t value);

#ifdef __cplusplus
}
#endif

inline uint32_t pb_rgba_get_pixel(pb_rgba *pb, const int x, const int y)
{
	return ((uint32_t *)(pb)->data)[(y*(pb)->pixelpitch) + x];
}

inline void pb_rgba_set_pixel(pb_rgba *pb, const int x, const int y, const int32_t value) 
{
	((uint32_t *)(pb)->data)[(y*(pb)->pixelpitch) + x] = value;
}



// Font information
typedef uint8_t cover_type;    //----cover_type
enum cover_scale_e
{
	cover_none = 0,                 //----cover_none 
	cover_shift = 8,                 //----cover_shift
	cover_size = 1 << cover_shift,  //----cover_size 
	cover_mask = cover_size - 1,    //----cover_mask 
	cover_full = cover_mask         //----cover_full 
};




typedef struct _font {
	uint8_t	height;			// height in pixels of all characters
	uint8_t baseline;		// baseline offset of character
	uint8_t start_char;		// ordinal of first character in the set
	uint8_t num_chars;		// number of characters in the font

	bool bigendian;			// endianness of current machine
	uint8_t *charbits;		// pointer to where the bits for the chars start

} font_t;

// Rectangle representing the boundary of a glyph
struct glyph_rect
{
	int x1, y1, x2, y2;
	double dx, dy;
};

typedef struct _glyph {
	size_t width;
	size_t byte_width;

	uint8_t *data;
} glyph_t;


#ifdef __cplusplus
extern "C" {
#endif

	void font_t_init(font_t *f, const uint8_t *data);


	void glyph_t_prepare(const font_t *font, const glyph_t *ginfo, struct glyph_rect* r, double x, double y, bool flip);
	void glyph_t_init(const font_t *f, glyph_t *ginfo, const unsigned int glyph);
	size_t font_t_glyph_width(const font_t *f, const unsigned int glyph);
	size_t font_t_str_width(const font_t *f, const char * str);
	//void font_str_box(const font_t *f, const char * str, int *x, int *y, int *swidth, int *sheight);

	void glyph_t_span(const font_t *f, glyph_t *g, unsigned i, cover_type *m_span);
	int scan_glyph(pb_rgba *pb, font_t *font, glyph_t *glyph, const int x, const int y, const uint32_t color);
	int scan_str(pb_rgba *pb, font_t *font, const int x, const int y, const char *chars, const uint32_t color);

#ifdef __cplusplus
}
#endif

// PBM Handling
#ifdef __cplusplus
extern "C" {
#endif

	DPROC_API int PPM_read_binary(const char *filename, pb_rgba *fb);			// will read P6
	DPROC_API int write_PPM_binary(const char *filename, pb_rgba *fb);	// write a P6 file
	DPROC_API int write_PPM_ascii(const char *filename, pb_rgba *fb);		// write a P3 file

#ifdef __cplusplus
}
#endif
