#pragma once

#include "dproc_config.h"

typedef float	float32;
typedef double	float64;
typedef float32 real;
typedef float coord;

typedef real real2[2];
typedef real real3[3];
typedef real real4[4];






typedef enum {
	COLOR_MODE_RGB,
	COLOR_MODE_HSB
} COLORMODE;

typedef enum {
	CORNER,
	CORNERS,
	RADIUS,
	CENTER
} RECTMODE;

typedef enum {
	TX_CENTER = 0x00,
	TX_LEFT = 0x01,
	TX_RIGHT = 0x04,
	TX_TOP = 0x10,
	TX_BOTTOM = 0x40

} ALIGNMENT;

typedef enum {
	GR_POINTS,
	GR_LINES,
	GR_LINE_STRIP,
	GR_LINE_LOOP,
	GR_POLYGON,
	GR_QUADS,
	GR_QUAD_STRIP,
	GR_TRIANGLES,
	GR_TRIANGLE_STRIP,
	GR_TRIANGLE_FAN
} GEOMETRY;

typedef enum {
	STROKE,
	CLOSE
} KindOfClose;


struct COLOR
{
	COLOR() :value(0) {}
	//COLOR(const COLOR &other) :value(other.value){}
	COLOR(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) :r(r), g(g), b(b), a(a) {}

	union {
		struct {
			uint8_t b, g, r, a;
		};
		uint32_t value;
	};
};

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


struct Vector2d
{
	Vector2d(coord x, coord y)
		:_x(x), _y(y)
	{};

	coord _x;
	coord _y;
};

typedef struct {
	float x;
	float y;
	float z;
} Pt3;


// pixel buffer rectangle
class pb_rect
{
public:
	int x, y;
	int width, height;

	pb_rect() :
		x(0), y(0),
		width(0), height(0)
	{}

	pb_rect(const int x, const int y,
		const int gWidth, const int gHeight) :
		x(x), y(y),
		width(gWidth), height(gHeight)
	{}

	bool containsPoint(const float x, const float y);
	bool containsRect(const pb_rect &other);
	pb_rect intersection(const pb_rect &b);
	bool intersects(const pb_rect &b);
	void clear();
};

/*
A pixel buffer is an array of pixels.

The pixelformat determines the layout of the
individual elements.

data		- pointer to the beginning of the data
bitStride	- number of bits between rows
pixelpitch	- number of pixels between rows
*/

typedef struct _pb_rgba {
	uint8_t *		data;			// pointer to the actual data
	unsigned int	pixelpitch;		// how many pixels per row, if alias
	int				bitStride;		// how many bits per row
	int				owndata;		// does this struct own the data pointer
	pb_rect			frame;			// if alias, what portion of parent
} pb_rgba;

struct PImage
{
	pb_rgba fb;
};


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


// Linear Algebra types
typedef struct _mat2 {
	real m11, m12;
	real m21, m22;
} mat2;

typedef struct _mat3 {
	real m11, m12, m13;
	real m21, m22, m23;
	real m31, m32, m33;
} mat3;


typedef struct _mat4 {
	real m11, m12, m13, m14;
	real m21, m22, m23, m24;
	real m31, m32, m33, m34;
	real m41, m42, m43, m44;
} mat4;

