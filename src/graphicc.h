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



#include "dproc_config.h"
#include "dproc_constants.h"
#include "dptypes.h"
#include "dpdevice.h"
#include "dp_pixmanip.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// This indicates that the machine the code is running on
// is a little endian machine, therefore, when laying out
// the values of a pixel in terms of bytes, the blue component
// is in the least significant byte of a 4 byte integer value.
#define BGR_DOMINANT 1


#include "dproc_types.h"


inline bool isBigEndian() {int t = 1;return (*(char*)&t == 0);}

inline real DEGREES(const real radians) { return (real)(G_RTOD * radians); }
inline real RADIANS(const real degrees) { return (real)(G_DTOR * degrees); }

// utils
#define min3(a,b,c) __min(__min(a,b),c)
#define max3(a,b,c) __max(__max(a,b),c)
#define MIN(a,b) __min(a,b)
#define MAX(a,b) __max(a,b)


// Map a value from one range to another
// A fraction is created to relate value (a) to the range (rlo - rhi).  This fraction
// is then applied to the second range (slo - shi).
// This will work for values of a both inside and outside the range itself
inline double MAP(double a, double rlo, double rhi, double slo, double shi) { return slo + (a - rlo) * (shi - slo) / (rhi - rlo); }
#define map MAP

// Return a value between the rlo and rhi
inline double CLAMP(double a, double rlo, double rhi){ return a < rlo ? rlo : (a>rhi ? rhi : a); }


// turn a division by 255 into something 
// much cheaper to calculate
// for values between 0 and 65534
#define div255(num) ((num + (num >> 8)) >> 8)

// perform a linear interpolation between a value 'a'
// a background value, and a foreground value, using
// fast div255
#define lerp255(bg, fg, a) ((uint8_t)div255((fg*a+bg*(255-a))))

// returns the sign of the value
// value  < 0 --> -1
// value  > 0 -->  1
// value == 0 -->  0
// this will only work in cases where 0 represents false
// and 1 represents true
inline int sgn(real val) { return ((0 < val) - (val < 0)); }





#ifdef BGR_DOMINANT
inline uint32_t RGBA(const int r, const int g, const int b, const int a) 
{
	return RGBA2PIXELABGR(r, g, b, a);
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





#ifdef __cplusplus
extern "C" {
#endif

DPROC_API int pb_rgba_init(pb_rgba *pb, const unsigned int width, const unsigned int height);
DPROC_API int pb_rgba_free(pb_rgba *pb);

DPROC_API int pb_rgba_get_frame(pb_rgba *src, const DPCOORD x, const DPCOORD y, const size_t width, const size_t height, pb_rgba *pf);

DPROC_API void pb_rgba_cover_pixel(pb_rgba *pb, const DPCOORD x, const DPCOORD y, const DPPIXELVAL value);

#ifdef __cplusplus
}
#endif

inline DPPIXELVAL pb_rgba_get_pixel(pb_rgba *pb, const DPCOORD x, const DPCOORD y)
{
	return ((uint32_t *)(pb)->data)[(y*(pb)->pixelpitch) + x];
}

inline void pb_rgba_set_pixel(pb_rgba *pb, const DPCOORD x, const DPCOORD y, const DPPIXELVAL value)
{
	((uint32_t *)(pb)->data)[(y*(pb)->pixelpitch) + x] = value;
}






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
