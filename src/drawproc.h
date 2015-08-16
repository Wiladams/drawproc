#pragma once

//#define BGR_DOMINANT 1

#include "animwin32.h"

#include "graphicc.h"
#include "linearalgebra.h"
#include "raster_rgba.h"
#include "vkeys.h"

#include "agg_embedded_raster_fonts.h"

#define radians RADIANS

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*EventObserverHandler)();


// IO Event Handlers
DPROC_API void keyPressed();
DPROC_API void keyReleased();
DPROC_API void keyTyped();

DPROC_API void mouseClicked();
DPROC_API void mouseDragged();
DPROC_API void mouseMoved(); 
DPROC_API void mousePressed();
DPROC_API void mouseReleased();

#ifdef __cplusplus
}
#endif

void redraw();

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

typedef float coord;

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
extern size_t width;
extern size_t height;

extern pb_rgba *gpb;
extern pb_rect pixelFrame;


extern COLOR bgColor;
extern pb_rgba *bgImage;

extern RECTMODE grectMode;
extern RECTMODE gellipseMode;

// Initialization
void init();
void noLoop();
void loop();

// time
uint64_t millis();

// Drawing API
void size(const size_t width, const size_t height);



// Mouse
// Mouse
extern int mouseX;
extern int mouseY;
extern bool isMousePressed;
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
extern int key;
extern int keyCode;
extern bool isKeyPressed;
// keyPressed()
// keyReleased()
// keyTyped()

// Setting colors
void colorMode(const COLORMODE mode, const float max1=-1, const float max2=-1, const float max3=-1, const float maxA=-1);
COLOR color(const float v1, const float v2=-1, const float v3=-1, const float alpha=-1);

void background(const float v1, const float v2 = -1, const float v3 = -1, const float alpha = -1);
void background(const COLOR value);
void backgroundImage(pb_rgba *bg);
//void clear();

//void colorMode();
void noFill();
void fill(const float v1, const float v2 = -1, const float v3 = -1, const float alpha = -1);
void fill(const COLOR value);

void noStroke();
void stroke(const COLOR value);
void stroke(const float v1, const float v2 = -1, const float v3 = -1, const float alpha=-1);

// attributes
void ellipseMode(const RECTMODE mode);
void noSmooth();
void rectMode(const RECTMODE mode);
void smooth();
//void strokeCap();
//void strokeJoin();
void strokeWeight(const float weight);


// 2D primitives
void bezier(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int segments=60);
void ellipse(const float a, const float b, const float c, const float d);
void line(const int x1, const int y1, const int x2, const int y2);
void lineloop(const size_t nPtr, const int *pts);
void point(const int x, const int y);
void rect(const int a, const int b, const int c, const int d);
void quad(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int x4, const int y4);
void triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3);
void polygon(int nverts, int *a);

// Text
extern font_t gfont;

// createFont
// loadFont()
void setFont(const uint8_t *fontdata);
void text(const char *str, const int x, const int y);
//void textFont(font_t *font);
void textAlign(const int alignX=TX_LEFT, const int alignY = TX_BOTTOM);
// textLeading()
// textMode()
void textSize(const int size);
// textWidth()
// textAscent()
// textDescent()

// Shape
void beginShape(const int shapeKind = GR_POLYGON);
void vertex(const int x, const int y);
void bezierVertex(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3);
void endShape(const int kindOfClose = STROKE);

// Images
void image(const PImage *img, const float a, const float b, const float c=-1, const float d=-1);
PImage * loadImage(const char *filename, const char *extension = nullptr);

// Math
double random(const float low, const float high);
double random(const float high);
inline double sq(const double value) { return value*value; }

// Coordinate transformation
void applyMatrix();
void popMatrix();
void printMatrix();
void pushMatrix();
void resetMatrix();
void rotate(const float angle, const coord x, const coord y, const coord z);
void rotateX(const float anglex);
void rotateY(const float angley);
void rotateZ(const float anglez);
void scale(const float a, const float b, const float c);
void shearX();
void shearY();
void shearZ();
void translate(const coord x, const coord y, const coord z = 0);


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

