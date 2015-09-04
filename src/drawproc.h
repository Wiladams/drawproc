#pragma once

#include "graphicc.h"
#include "linearalgebra.h"
#include "raster_rgba.h"
#include "vkeys.h"

#include "agg_embedded_raster_fonts.h"


#define radians RADIANS

#ifdef __cplusplus
extern "C" {
#endif

// These should be implemented by a module to be loaded
DPROC_EXPORT void draw();
DPROC_EXPORT void setup();


// IO Event Handlers
DPROC_EXPORT void keyPressed();
DPROC_EXPORT void keyReleased();
DPROC_EXPORT void keyTyped();

DPROC_EXPORT void mouseClicked();
DPROC_EXPORT void mouseDragged();
DPROC_EXPORT void mouseMoved();
DPROC_EXPORT void mousePressed();
DPROC_EXPORT void mouseReleased();

#ifdef __cplusplus
}
#endif

DPROC_API void redraw();

/*
Drawing API
*/
struct COLOR
{
	COLOR() :value(0){}
	//COLOR(const COLOR &other) :value(other.value){}
	COLOR(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a):r(r),g(g),b(b),a(a){}

	union {
		struct {
			uint8_t b, g, r, a;
		};
		uint32_t value;
	};
};

typedef enum COLORMODE {
	COLOR_MODE_RGB,
	COLOR_MODE_HSB
};

typedef enum RECTMODE{
	CORNER,
	CORNERS,
	RADIUS,
	CENTER
};

typedef enum ALIGNMENT {
	TX_CENTER	= 0x00,
	TX_LEFT		= 0x01,
	TX_RIGHT	= 0x04,
	TX_TOP		= 0x10,
	TX_BOTTOM	= 0x40

};

typedef enum GEOMETRY {
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
};

typedef enum KindOfClose {
	STROKE,
	CLOSE
};


struct Vector2d
{
	Vector2d(coord x, coord y)
	:_x(x), _y(y)
	{};

	coord _x;
	coord _y;
};

struct Vector3d
{
	union {
		struct {
			coord _x, _y, _z;
		};
		coord _v[3];
	};
};

struct PImage
{
	pb_rgba fb;
};



// Global variables
DPROC_API extern size_t width;
DPROC_API extern size_t height;


DPROC_API extern pb_rgba *gpb;
DPROC_API extern pb_rect pixelFrame;


DPROC_API extern COLOR bgColor;
DPROC_API extern pb_rgba *bgImage;

DPROC_API extern RECTMODE grectMode;
DPROC_API extern RECTMODE gellipseMode;

// Initialization
//DPROC_API void init();


DPROC_API void noLoop();
DPROC_API void loop();

// time
DPROC_API uint64_t millis();

// Drawing API
DPROC_API void size(const size_t width, const size_t height);



// Mouse
// Mouse
DPROC_API extern int mouseX;
DPROC_API extern int mouseY;
DPROC_API extern bool isMousePressed;
// pmouseX
// pmouseY
// mouseButton

// mouseClicked()
// mouseDragged()
// mouseMoved()
// mousePressed()
// mouseReleased()
// mouseWheel()



// Keyboard
DPROC_API extern int keyChar;
DPROC_API extern int keyCode;
DPROC_API extern bool isKeyPressed;


// Setting colors
DPROC_API void colorMode(const COLORMODE mode, const float max1 = -1, const float max2 = -1, const float max3 = -1, const float maxA = -1);
DPROC_API COLOR color(const float v1, const float v2 = -1, const float v3 = -1, const float alpha = -1);

DPROC_API void background(const float v1, const float v2 = -1, const float v3 = -1, const float alpha = -1);
DPROC_API void background(const COLOR value);
DPROC_API void backgroundImage(pb_rgba *bg);
//void clear();

//void colorMode();
DPROC_API void noFill();
DPROC_API void fill(const float v1, const float v2 = -1, const float v3 = -1, const float alpha = -1);
DPROC_API void fill(const COLOR value);

DPROC_API void noStroke();
DPROC_API void stroke(const COLOR value);
DPROC_API void stroke(const float v1, const float v2 = -1, const float v3 = -1, const float alpha = -1);

// attributes
DPROC_API void ellipseMode(const RECTMODE mode);
DPROC_API void noSmooth();
DPROC_API void rectMode(const RECTMODE mode);
DPROC_API void smooth();
//DPROC_API void strokeCap();
//DPROC_API void strokeJoin();
DPROC_API void strokeWeight(const float weight);


// 2D primitives
DPROC_API void bezier(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int segments = 60);
DPROC_API void ellipse(const float a, const float b, const float c, const float d);
DPROC_API void line(const int x1, const int y1, const int x2, const int y2);
DPROC_API void lineloop(const size_t nPtr, const int *pts);
DPROC_API void point(const int x, const int y);
DPROC_API void rect(const int a, const int b, const int c, const int d);
DPROC_API void quad(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int x4, const int y4);
DPROC_API void triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3);
DPROC_API void polygon(int nverts, int *a);

// Text
extern font_t gfont;

// createFont
// DPROC_API loadFont()
DPROC_API void setFont(const uint8_t *fontdata);
DPROC_API void text(const char *str, const int x, const int y);
//DPROC_API void textFont(font_t *font);
DPROC_API void textAlign(const int alignX = TX_LEFT, const int alignY = TX_BOTTOM);
// DPROC_API textLeading()
// DPROC_API textMode()
DPROC_API void textSize(const int size);
// DPROC_API textWidth()
// DPROC_API textAscent()
// DPROC_API textDescent()

// Shape
DPROC_API void beginShape(const int shapeKind = GR_POLYGON);
DPROC_API void vertex(const int x, const int y);
DPROC_API void bezierVertex(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3);
DPROC_API void endShape(const int kindOfClose = STROKE);

// Images
DPROC_API void image(const PImage *img, const float a, const float b, const float c = -1, const float d = -1);
DPROC_API PImage * loadImage(const char *filename, const char *extension = nullptr);

// Math
DPROC_API double random(const float low, const float high);
DPROC_API double random(const float high);
DPROC_API inline double sq(const double value) { return value*value; }

// Coordinate transformation
DPROC_API void applyMatrix();
DPROC_API void popMatrix();
DPROC_API void printMatrix();
DPROC_API void pushMatrix();
DPROC_API void resetMatrix();
DPROC_API void rotate(const float angle, const coord x, const coord y, const coord z);
DPROC_API void rotateX(const float anglex);
DPROC_API void rotateY(const float angley);
DPROC_API void rotateZ(const float anglez);
DPROC_API void scale(const float a, const float b, const float c);
DPROC_API void shearX();
DPROC_API void shearY();
DPROC_API void shearZ();
DPROC_API void translate(const coord x, const coord y, const coord z = 0);


#define pRed		COLOR(255, 0, 0, 255)
#define pGreen		COLOR(0, 255, 0, 255)
#define pBlue		COLOR(0, 0, 255, 255)

#define pBlack		COLOR(0, 0, 0, 255)
#define pWhite		COLOR(255, 255, 255, 255)
#define pYellow		COLOR(255, 255, 0, 255)
#define pTurquoise	COLOR(0, 255, 255, 255)
#define pDarkGray	COLOR(93, 93, 93, 255)
#define pGray		COLOR(127,127,127,255)
#define pLightGray	COLOR(163, 163, 163, 255)

#define aliceblue	COLOR(240, 248, 255, 255)
#define cornsilk	COLOR(255, 248, 220, 255)

