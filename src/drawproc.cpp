
#include "animwin32.h"
#include "drawproc.h"
#include "bezier.h"

//#include <windowsx.h>	// GET_X_LPARAM

#include <stack>
#include <vector>
#include <math.h>
#include <cstdlib>

// Globals for the environment
pb_rgba *gpb;
size_t width = 0;
size_t height = 0;
pb_rect pixelFrame;
unsigned int frameCount = 0;

RECTMODE grectMode = CORNER;
RECTMODE gellipseMode = CENTER;


// color setting
COLORMODE gColorMode = COLOR_MODE_RGB;
float gColorMax1 = 255;
float gColorMax2 = 255;
float gColorMax3 = 255;
float gColorMaxA = 255;

COLOR bgColor = COLOR(93, 93, 93, 255);		// pDarkGray;
pb_rgba *bgImage = nullptr;
COLOR strokeColor = COLOR(0, 0, 0, 255);
float gstrokeWeight = 1;
COLOR fillColor = COLOR(255, 255, 255, 255);

// Text Settings
font_t gfont;
int gTextSize;
int gTextAlignX;
int gTextAlignY;

// Keyboard
int keyCode = 0;
int keyChar = 0;
bool isKeyPressed = 0;

// Mouse
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

int mouseX = 0;
int mouseY = 0;
bool isMousePressed = false;
int mouseButton = 0;


// Typedef an STL vector of vertices which are used to represent
// a polygon/contour and a series of triangles.
typedef std::vector< Vector2d > Vector2dVector;
typedef std::stack< mat3> TransformStack;

static TransformStack gtransStack;

// Time
uint64_t millis()
{
	return (uint64_t)seconds() * 1000;
}

typedef void(*EventObserverHandler)();

static EventObserverHandler gkbdOnPressedHandler = nullptr;
static EventObserverHandler gkbdOnReleasedHandler = nullptr;
static EventObserverHandler gkbdOnTypedHandler = nullptr;


static EventObserverHandler gOnMouseClickedHandler = nullptr;
static EventObserverHandler gOnMousePressedHandler = nullptr;
static EventObserverHandler gmouseReleasedHandler = nullptr;
static EventObserverHandler gmouseOnWheelHandler = nullptr;
static EventObserverHandler gmouseOnDraggedHandler = nullptr;
static EventObserverHandler gmouseOnMovedHandler = nullptr;


void setOnKeyPressedHandler(EventObserverHandler handler)
{
	gkbdOnPressedHandler = handler;
}

void setOnKeyReleasedHandler(EventObserverHandler handler)
{
	gkbdOnReleasedHandler = handler;
}

void setOnKeyTypedHandler(EventObserverHandler handler)
{
	gkbdOnTypedHandler = handler;
}



LRESULT CALLBACK keyHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

		case WM_CHAR:
			// Processing regular characters, after translation of various keycodes
			keyChar = wParam;

			if (gkbdOnTypedHandler) {
				gkbdOnTypedHandler();
				//hWnd, message, wParam, lParam);
			}

			switch (keyChar){
				case 0x1B:  // ESC
					quit();
				break;
			}
		break;

		case WM_KEYDOWN:
			keyCode = wParam;
			isKeyPressed = true;

			if (gkbdOnPressedHandler) {
				gkbdOnPressedHandler();
					//hWnd, message, wParam, lParam);
			}
		break;

		case WM_KEYUP:
			// raw keycodes
			keyCode = wParam;
			isKeyPressed = false;

			if (gkbdOnReleasedHandler) {
				gkbdOnReleasedHandler();
			}
		break;


		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


// Mouse Routines
void setOnMouseClickedHandler(EventObserverHandler handler)
{
	gOnMouseClickedHandler = handler;
}

void setOnMousePressedHandler(EventObserverHandler handler)
{
	gOnMousePressedHandler = handler;
}

void setOnMouseReleasedHandler(EventObserverHandler handler)
{
	gmouseReleasedHandler = handler;
}

void setOnMouseWheelHandler(EventObserverHandler handler)
{
	gmouseOnWheelHandler = handler;
}

void setOnMouseDraggedHandler(EventObserverHandler handler)
{
	gmouseOnDraggedHandler = handler;
}

void setOnMouseMovedHandler(EventObserverHandler handler)
{
	gmouseOnMovedHandler = handler;
}


LRESULT CALLBACK mouseHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_MOUSEWHEEL:
			//int fwKeys = GET_KEYSTATE_WPARAM(wParam);
			//int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

			if (gmouseOnWheelHandler != nullptr) {
				gmouseOnWheelHandler(); // hWnd, message, wParam, lParam);
			}
		break;


		case WM_MOUSEMOVE:
			mouseX = GET_X_LPARAM(lParam);
			mouseY = GET_Y_LPARAM(lParam);

			if (isMousePressed) {
				if (gmouseOnDraggedHandler != nullptr) {
					gmouseOnDraggedHandler();
				}
			} else if (gmouseOnMovedHandler != nullptr) {
				gmouseOnMovedHandler();
			}
		break;

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			isMousePressed = true;
			mouseButton = wParam;

			if (gOnMousePressedHandler != nullptr) {
				gOnMousePressedHandler();
			}
		break;

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			isMousePressed = false;

			if (gmouseReleasedHandler != nullptr) {
				gmouseReleasedHandler();
			}

			if (gOnMouseClickedHandler != nullptr) {
				gOnMouseClickedHandler();
			}
		break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


void initFont()
{
	// Setup text
	font_t_init(&gfont, verdana12);
	gTextSize = 10;
	gTextAlignX = TX_LEFT;
	gTextAlignY = TX_TOP;
}

void initInput()
{
	//HMODULE modH = GetModuleHandle(NULL);
	HMODULE clientModule = getClientModule();

	// Keyboard handling routines
	setOnKeyPressedHandler((EventObserverHandler)GetProcAddress(clientModule, "keyPressed"));
	setOnKeyReleasedHandler((EventObserverHandler)GetProcAddress(clientModule, "keyReleased"));
	setOnKeyTypedHandler((EventObserverHandler)GetProcAddress(clientModule, "keyTyped"));

	setKeyboardHandler(keyHandler);


	// Mouse Handling Routines
	setOnMousePressedHandler((EventObserverHandler)GetProcAddress(clientModule, "mouseClicked"));
	setOnMousePressedHandler((EventObserverHandler)GetProcAddress(clientModule, "mousePressed"));
	setOnMouseReleasedHandler((EventObserverHandler)GetProcAddress(clientModule, "mouseReleased"));
	setOnMouseMovedHandler((EventObserverHandler)GetProcAddress(clientModule, "mouseMoved"));
	setOnMouseDraggedHandler((EventObserverHandler)GetProcAddress(clientModule, "mouseDragged"));

	setMouseHandler(mouseHandler);
}

// size of window
void size(const size_t lwidth, const size_t lheight)
{
	width = lwidth;
	height = lheight;

	pixelFrame.x = 0;
	pixelFrame.y = 0;
	pixelFrame.width = (float)width;
	pixelFrame.height = (float)height;

	initFont();

	void *data = SetWindowSize(lwidth, lheight);

	// Create the global pixel buffer
	// Save the pointer into the pb_rgba pixel buffer
	gpb = new pb_rgba();
	gpb->data = (uint8_t *)data;
	gpb->owndata = false;
	gpb->pixelpitch = width;
	gpb->frame.height = height;
	gpb->frame.width = width;
	gpb->frame.x = 0;
	gpb->frame.y = 0;

	// paint the background at least once before beginning
	raster_rgba_rect_fill(gpb, 0, 0, width, height, RGBA(0,0,0,255));

	initInput();
}

void noLoop()
{
	setDrawInLoop(false);
}

void loop()
{
	setDrawInLoop(true);
}

void redraw()
{
	forceDraw();
}

// Coordinate transformation
/*
void applyMatrix();
void popMatrix();
void printMatrix();
void pushMatrix();
void resetMatrix();
void rotate();
void rotateX();
void rotateY();
void rotateZ();
void scale();
void shearX();
void shearY();
void shearZ();
*/
void translate(const coord x, const coord y, const coord z)
{

}


// Math
double dist(int x1, int y1, int x2, int y2)
{
	return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

double random(const float low, const float high)
{
	double frac = (double)rand() / RAND_MAX;
	double ret = low + frac * (high - low);
	
	return ret;
}

double random(const float rndMax)
{
	return random(0, rndMax);
}

// color setting
// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)


void RGBtoHSV(float r, float g, float b, float *h, float *s, float *v)
{
	float minv, maxv, delta;

	minv = min3(r, g, b);
	maxv = max3(r, g, b);
	*v = maxv;				// v

	delta = maxv - minv;

	if (maxv != 0)
		*s = delta / maxv;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		*s = 0;
		*h = -1;
		return;
	}

	if (r == maxv)
		*h = (g - b) / delta;		// between yellow & magenta
	else if (g == maxv)
		*h = 2 + (b - r) / delta;	// between cyan & yellow
	else
		*h = 4 + (r - g) / delta;	// between magenta & cyan

	*h *= 60;				// degrees
	if (*h < 0)
		*h += 360;

}

void HSVtoRGB(float *r, float *g, float *b, float h, float s, float v)
{
	int i;
	float f, p, q, t;

	if (s == 0) {
		// achromatic (gray)
		*r = *g = *b = v;
		return;
	}

	h /= 60;			// sector 0 to 5
	i = floor(h);
	f = h - i;			// factorial part of h
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));

	switch (i) {
	case 0:
		*r = v;
		*g = t;
		*b = p;
		break;
	case 1:
		*r = q;
		*g = v;
		*b = p;
		break;
	case 2:
		*r = p;
		*g = v;
		*b = t;
		break;
	case 3:
		*r = p;
		*g = q;
		*b = v;
		break;
	case 4:
		*r = t;
		*g = p;
		*b = v;
		break;
	default:		// case 5:
		*r = v;
		*g = p;
		*b = q;
		break;
	}

}

COLOR HSBA(float h, float s, float v, float a)
{
	float r=0, g=0, b=0;
	HSVtoRGB(&r, &g, &b, h, s, v);

	return COLOR(r*255, g*255, b*255, (int)a);
}

void colorMode(const COLORMODE mode, const float max1, const float max2, const float max3, const float maxA)
{
	gColorMode = mode;
	if (max1 == -1) {
		return; 
	}

	if (max2 == -1 && max3 == -1 && maxA == -1) {
		gColorMax1 = max1;
		gColorMax2 = max1;
		gColorMax3 = max1;
		gColorMaxA = max1;
	}
	else if (maxA == -1) {
		gColorMax1 = max1;
		gColorMax2 = max2;
		gColorMax3 = max3;
	}
	else {
		gColorMax1 = max1;
		gColorMax2 = max2;
		gColorMax3 = max3;
		gColorMaxA = maxA;
	}
}

COLOR colorFromRGBA(const float v1, const float v2, const float v3, const float alpha)
{
	if ((v1 != -1) && (v2 == -1) && (v3 == -1) && (alpha == -1)) {
		//color(gray)
			return COLOR(
				MAP(v1, 0, gColorMax1, 0, 255), 
				MAP(v1, 0, gColorMax1, 0, 255), 
				MAP(v1, 0, gColorMax1, 0, 255), 
				(int)gColorMaxA);
	
	} else if ((v1 != -1) && (v2 != -1) && (v3 == -1) && (alpha == -1)) {
		//color(v1, alpha)
		return COLOR(
			MAP(v1, 0, gColorMax1, 0, 255),
			MAP(v1, 0, gColorMax1, 0, 255),
			MAP(v1, 0, gColorMax1, 0, 255),
			MAP(v2, 0, gColorMaxA, 0, 255));
	}
	else if ((v1 != -1) && (v2 != -1) && (v3 != -1) && (alpha == -1)) {
		//color(v1, v2, v3)
		return COLOR(
			MAP(v1, 0, gColorMax1, 0, 255),
			MAP(v2, 0, gColorMax2, 0, 255),
			MAP(v3, 0, gColorMax3, 0, 255),
			(int)gColorMaxA);
	}
	else {
		// color(v1, v2, v3, alpha)
		if (gColorMode == COLOR_MODE_RGB) {
			return COLOR(
				(int)MAP(v1, 0, gColorMax1, 0, 255), 
				(int)MAP(v2, 0, gColorMax2, 0, 255), 
				(int)MAP(v3, 0, gColorMax3, 0, 255), 
				(int)MAP(alpha, 0, gColorMaxA, 0, 255));
		}
	}

	return COLOR();
}

COLOR colorFromHSBA(const float v1, const float v2, const float v3, const float alpha)
{
	if ((v1 != -1) && (v2 == -1) && (v3 == -1) && (alpha == -1)) {
		//color(v)
		return HSBA(0, 0, 
			MAP(v1, 0, gColorMax3, 0, 1), 
			MAP(gColorMaxA, 0, gColorMaxA, 0, 255));
	}
	else if ((v1 != -1) && (v2 != -1) && (v3 == -1) && (alpha == -1)) {
		//color(v, alpha)
		return HSBA(0, 0, 
			MAP(v1, 0, gColorMax3, 0, 1), 
			MAP(v2, 0, gColorMaxA, 0, 255));
	}
	else if ((v1 != -1) && (v2 != -1) && (v3 != -1) && (alpha == -1)) {
		//color(h, s, v)
		return HSBA(
			MAP(v1, 0, gColorMax1, 0, 360),
			MAP(v2, 0, gColorMax2, 0, 1),
			MAP(v3, 0, gColorMax3, 0, 1),
			MAP(gColorMaxA, 0, gColorMaxA, 0, 255));

	}
	else if ((v1 != -1) && (v2 != -1) && (v3 != -1) && (alpha != -1)) {
		//color(h, s, v, a)
		return HSBA(
			MAP(v1, 0, gColorMax1, 0, 360),
			MAP(v2, 0, gColorMax2, 0, 1),
			MAP(v3, 0, gColorMax3, 0, 1),
			MAP(alpha, 0, gColorMaxA, 0, 255));
	}

	return COLOR();
}

COLOR color(const float v1, const float v2, const float v3, const float alpha)
{
	if (gColorMode == COLOR_MODE_RGB) {
		return colorFromRGBA(v1, v2, v3, alpha);
	}

	return colorFromHSBA(v1, v2, v3, alpha);
}

void background(const float v1, const float v2, const float v3, const float alpha)
{
	background(color(v1, v2, v3, alpha));
}

void background(const COLOR value)
{
	bgColor = value;
	if ((gpb != NULL) && (width > 0) && (height > 0))
	{
		raster_rgba_rect_fill(gpb, 0, 0, width, height, bgColor.value);
	}
}

void backgroundImage(pb_rgba *bg)
{
	raster_rgba_blit(gpb, 0, 0, bg);
}



void noFill()
{
	fillColor = COLOR();
}

void noStroke()
{
	strokeColor = COLOR();
}

void stroke(const float v1, const float v2, const float v3, const float alpha)
{
	strokeColor = color(v1, v2, v3, alpha);
}

void stroke(const COLOR value)
{
	strokeColor = value;
}

void noSmooth() {}
void smooth() {}
void strokeCap() {}
void strokeJoin() {}

void strokeWeight(const float weight)
{
	gstrokeWeight = weight;
}

void fill(const float v1, const float v2, const float v3, const float alpha)
{
	fillColor = color(v1, v2, v3, alpha);
}

void fill(const COLOR value)
{
	fillColor = value;
}

// 2D primitives
void polyline(pb_rgba *pb, Pt3 *curve, const int nPts, COLOR color)
{
	for (int idx = 0; idx < nPts; idx++) {
		raster_rgba_line_cover(pb, curve[idx].x, curve[idx].y, curve[idx + 1].x, curve[idx + 1].y, color.value);
	}
}

void bezier(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int segments)
{
	int nControls = 3;
	Pt3 controls[3] = { {x1,y1,0}, {x2,y2,0}, {x3,y3,0} };
	Pt3 *curve = (Pt3 *)malloc(segments*sizeof(Pt3));

	bez3_curve(controls, nControls, segments, curve);
	polyline(gpb, curve, segments, strokeColor);
}

void ellipseMode(const RECTMODE mode)
{
	gellipseMode = mode;
}

void ellipse(const float a, const float b, const float c, const float d)
{
	int x1 = 0, y1 = 0;
	int rwidth = 0, rheight = 0;

	switch (gellipseMode) {
	case CORNER:
		x1 = a;
		y1 = b;
		rwidth = c;
		rheight = d;
		break;

	case CORNERS:
		x1 = a;
		y1 = b;
		rwidth = c - a + 1;
		rheight = d - b + 1;
		break;

	case CENTER:
		x1 = a - c / 2;
		y1 = b - d / 2;
		rwidth = c;
		rheight = d;
		break;

	case RADIUS:
		x1 = a - c;
		y1 = b - d;
		rwidth = c * 2;
		rheight = d * 2;
		break;
	}


	size_t xradius = rwidth / 2;
	size_t yradius = rheight / 2;

	if (xradius == 0 || yradius == 0) {
		return;
	}

	uint32_t cx = x1+ rwidth/2;
	uint32_t cy = y1 + rheight/2;

	raster_rgba_ellipse_fill(gpb, cx, cy, xradius, yradius, fillColor.value);
	raster_rgba_ellipse_stroke(gpb, cx, cy, xradius, yradius, strokeColor.value);
}

void line(const int x1, const int y1, const int x2, const int y2)
{
	int xx1 = x1;
	int yy1 = y1;
	int xx2 = x2;
	int yy2 = y2;


	if (!clipLine(pixelFrame, xx1, yy1, xx2, yy2))
	{
		return;
	}

	raster_rgba_line_cover(gpb, xx1, yy1, xx2, yy2, strokeColor.value);
}

void lineloop(const Vector2dVector &pts)
{
	int nPts = pts.size();
	
	// we need at least 3 points to make a closed loop
	if (nPts < 3)
		return;

	for (int idx = 0; idx < nPts - 1; idx++)
	{
		line(pts[idx]._x, pts[idx]._y, pts[idx + 1]._x, pts[idx + 1]._y);
	}
	line(pts[nPts - 1]._x, pts[nPts - 1]._y, pts[0]._x, pts[0]._y);
}

void lineloop(const size_t nPts, const int *pts)
{
	if (nPts < 2)
		return;

	int firstx = pts[0];
	int firsty = pts[1];
	int lastx = firstx;
	int lasty = firsty;

	for (int idx = 1; idx < nPts; idx++)
	{
		int nextx = pts[idx * 2];
		int nexty = pts[(idx * 2) + 1];
		line(lastx, lasty, nextx, nexty);
		lastx = nextx;
		lasty = nexty;
	}

	// draw last point to beginning
	line(lastx, lasty, firstx, firsty);
}

void point(const int x, const int y)
{
	if (!pixelFrame.containsPoint(x, y))
		return;

	if (gstrokeWeight <= 1) {
		pb_rgba_cover_pixel(gpb, x, y, strokeColor.value);
	}
	else {
		// draw an ellipse centered at the specified point
		// with a width of the strokeWeight
		raster_rgba_ellipse_fill(gpb, x, y, gstrokeWeight / 2, gstrokeWeight / 2, strokeColor.value);
	}
}

void rectMode(const RECTMODE mode)
{
	grectMode = mode;
}

void rect(const int a, const int b, const int c, const int d)
{
	int x1 = 0, y1 = 0;
	int rwidth = 0, rheight = 0;

	switch (grectMode) {
	case CORNER:
		x1 = a;
		y1 = b;
		rwidth = c;
		rheight = d;
		break;

	case CORNERS:
		x1 = a;
		y1 = b;
		rwidth = c - a + 1;
		rheight = d - b + 1;
		break;

	case CENTER:
		x1 = a - c / 2;
		y1 = b - d / 2;
		rwidth = c;
		rheight = d;
		break;

	case RADIUS:
		x1 = a - c;
		y1 = b - d;
		rwidth = c * 2;
		rheight = d * 2;
		break;
	}

	// find the intersection of the rectangle and the pixelframe
	pb_rect crect = pixelFrame.intersection({ x1, y1, rwidth, rheight });

	if ((crect.width == 0) || (crect.height == 0))
		return;

	if (fillColor.value != 0) {
		raster_rgba_rect_fill_blend(gpb, crect.x, crect.y, crect.width, crect.height, fillColor.value);
	}

	// if the strokeColor != 0 then 
	// frame the rectangle in the strokeColor
	if (strokeColor.value != 0) {
		int pts[] = {
			crect.x, crect.y,
			crect.x, crect.y + crect.height - 1,
			crect.x + crect.width - 1, crect.y + crect.height - 1,
			crect.x + crect.width - 1, crect.y
		};

		lineloop(4, pts);
	}
}



void triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3)
{
	if (fillColor.value != 0) {
		raster_rgba_triangle_fill(gpb, x1, y1, x2, y2, x3, y3, fillColor.value);
	}

	if (strokeColor.value != 0) {
		int pts[] = {
			x1, y1,
			x2, y2,
			x3, y3
		};
		lineloop(3, pts);
	}
}

void quad(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int x4, const int y4)
{
	// preserve current stroke color
	COLOR savedStroke = strokeColor;

	noStroke();
	// triangle 1
	triangle(x1, y1, x2, y2, x4, y4);

	// triangle 2
	triangle(x2, y2, x3, y3, x4, y4);

	// outline
	stroke(savedStroke);

	if (strokeColor.value != 0) {
		int pts[] = {
			x1, y1,
			x2, y2,
			x3, y3,
			x4, y4
		};
		lineloop(4, pts);
	}
}


// polygon
//  Globals which should be set before calling this function:
//
//  int    polyCorners  =  how many corners the polygon has
//  float  polyX[]      =  horizontal coordinates of corners
//  float  polyY[]      =  vertical coordinates of corners
//  float  x, y         =  point to be tested
//
//
//  The function will return 1 if the point x,y is inside the polygon, or
//  0 if it is not.  If the point is exactly on the edge of the polygon,
//  then the function may return YES or NO.
//

int pointInPolygon(int polyCorners, int polyX[], int polyY[], const int x, const int y) 
{
	int   i, j = polyCorners - 1;
	int  oddNodes = 0;

	for (i = 0; i<polyCorners; i++) 
	{
		if ((polyY[i]< y && polyY[j] >= y
			|| polyY[j]< y && polyY[i] >= y)
			&& (polyX[i] <= x || polyX[j] <= x)) 
		{
			oddNodes ^= (polyX[i] + (y - polyY[i]) / (polyY[j] - polyY[i])*(polyX[j] - polyX[i])<x);
		}
		j = i;
	}

	return oddNodes;
}




// Draw a polygon

float triangle_area(const Vector2dVector &contour)
{
	int n = contour.size();

	float A = 0.0f;

	for (int p = n - 1, q = 0; q<n; p = q++)
	{
		A += contour[p]._x*contour[q]._y - contour[q]._x*contour[p]._y;
	}
	return A*0.5f;
}

/*
triangulate_inside_triangle
decides if a point P is Inside of the triangle
defined by A, B, C.
*/
bool triangle_pt_inside(float Ax, float Ay,
	float Bx, float By,
	float Cx, float Cy,
	float Px, float Py)

{
	float ax, ay, bx, by, cx, cy, apx, apy, bpx, bpy, cpx, cpy;
	float cCROSSap, bCROSScp, aCROSSbp;

	ax = Cx - Bx;  ay = Cy - By;
	bx = Ax - Cx;  by = Ay - Cy;
	cx = Bx - Ax;  cy = By - Ay;
	apx = Px - Ax;  apy = Py - Ay;
	bpx = Px - Bx;  bpy = Py - By;
	cpx = Px - Cx;  cpy = Py - Cy;

	aCROSSbp = ax*bpy - ay*bpx;
	cCROSSap = cx*apy - cy*apx;
	bCROSScp = bx*cpy - by*cpx;

	return ((aCROSSbp >= 0.0f) && (bCROSScp >= 0.0f) && (cCROSSap >= 0.0f));
};

bool triangulate_snip(const Vector2dVector &contour, int u, int v, int w, int n, int *V)
{
	int p;
	float Ax, Ay, Bx, By, Cx, Cy, Px, Py;

	Ax = contour[V[u]]._x;
	Ay = contour[V[u]]._y;

	Bx = contour[V[v]]._x;
	By = contour[V[v]]._y;

	Cx = contour[V[w]]._x;
	Cy = contour[V[w]]._y;

	if (EPSILON > (((Bx - Ax)*(Cy - Ay)) - ((By - Ay)*(Cx - Ax)))) return false;

	for (p = 0; p<n; p++)
	{
		if ((p == u) || (p == v) || (p == w)) continue;
		Px = contour[V[p]]._x;
		Py = contour[V[p]]._y;
		if (triangle_pt_inside(Ax, Ay, Bx, By, Cx, Cy, Px, Py)) return false;
	}

	return true;
}

bool triangulate_process(const Vector2dVector &contour)
{
	// allocate and initialize list of Vertices in polygon
	int n = contour.size();
	if (n < 3) return false;

	int *V = new int[n];

	// we want a counter-clockwise polygon in V

	if (0.0f < triangle_area(contour))
	for (int v = 0; v<n; v++) 
		V[v] = v;
	else
		for (int v = 0; v<n; v++) 
			V[v] = (n - 1) - v;

	int nv = n;

	/*  remove nv-2 Vertices, creating 1 triangle every time */
	int count = 2 * nv;   /* error detection */

	for (int m = 0, v = nv - 1; nv>2;)
	{
		/* if we loop, it is probably a non-simple polygon */
		if (0 >= (count--))
		{
			//** Triangulate: ERROR - probable bad polygon!
			return false;
		}

		/* three consecutive vertices in current polygon, <u,v,w> */
		int u = v; if (nv <= u) u = 0;     /* previous */
		v = u + 1; if (nv <= v) v = 0;     /* new v    */
		int w = v + 1; if (nv <= w) w = 0;     /* next     */

		if (triangulate_snip(contour, u, v, w, nv, V))
		{
			int a, b, c, s, t;

			/* true names of the vertices */
			a = V[u]; b = V[v]; c = V[w];

			// draw single triangle
			triangle(contour[a]._x, contour[a]._y, 
				contour[b]._x, contour[b]._y,
				contour[c]._x, contour[c]._y);

			m++;

			// remove v from remaining polygon
			for (s = v, t = v + 1; t<nv; s++, t++) 
				V[s] = V[t]; 
			nv--;

			// resest error detection counter
			count = 2 * nv;
		}
	}

	delete [] V;

	return true;
}

void polygon(const Vector2dVector &pts)
{
	COLOR oldStroke = strokeColor;

	//  Invoke the triangulator to render the polygon as triangles
	noStroke();
	bool processed = triangulate_process(pts);

	stroke(oldStroke);
	if (processed) {
		lineloop(pts);
	}
}

void polygon(int nverts, int *verts)
{
	Vector2dVector a;
	for (int idx = 0; idx < nverts; idx++){
		a.push_back(Vector2d(verts[idx*2+0], verts[idx*2+1]));
	}


	//  Invoke the triangulator to render the polygon as triangles
	noStroke();
	bool processed = triangulate_process(a);
		
	stroke(strokeColor);
	if (processed) {
		// draw the outline
		lineloop(a);
	}
}


// Shape
static int gShapeKind = GR_POLYGON;
Vector2dVector gShape;



void beginShape(const int shapeKind)
{
	gShapeKind = shapeKind;
}

void vertex(const int x, const int y)
{
	gShape.push_back(Vector2d(x,y));
}

void bezierVertex(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3)
{
	int nControls = 4;
	int segments = 120;
	Pt3 controls[4] = { {gShape[0]._x, gShape[0]._y, 0},{ x1, y1, 0 }, { x2, y2, 0 }, { x3, y3, 0 } };
	Pt3 *curve = (Pt3 *)malloc(segments*sizeof(Pt3));

	bez3_curve(controls, nControls, segments, curve);
	for (int idx = 0; idx < segments; idx++) {
		vertex(curve[idx].x, curve[idx].y);
	}

}

void endShape(const int kindOfFinish)
{
	int n = gShape.size();
	COLOR oldStroke = strokeColor;

	switch (gShapeKind) {
		case GR_POINTS:
		for (int idx = 0; idx < n; idx++){
			point(gShape[idx]._x, gShape[idx]._y);
		}
		break;

		case GR_LINES: {
			if (kindOfFinish == STROKE) {
				int nLines = n / 2;
				for (int idx = 0; idx < nLines; idx++)
				{
					line(gShape[idx * 2]._x, gShape[idx * 2]._y, gShape[(idx * 2) + 1]._x, gShape[(idx * 2) + 1]._y);
				}
			}
			else if (kindOfFinish == CLOSE) {
				polygon(gShape);
			}
		} 
		break;
	
		case GR_LINE_STRIP:
			for (int idx = 0; idx < n - 1; idx++) {
				line(gShape[idx]._x, gShape[idx]._y, gShape[idx + 1]._x, gShape[idx + 1]._y);
			}
		break;

		case GR_LINE_LOOP:
			lineloop(gShape);
		break;

		case GR_POLYGON: {
			polygon(gShape);
		}
		break;
		
		case GR_QUADS: {
			int nTris = n / 4;
			for (int idx = 0; idx < nTris; idx++) {
				int offset = idx * 4;
				quad(gShape[offset]._x, gShape[offset]._y,
					gShape[(offset)+1]._x, gShape[(offset)+1]._y,
					gShape[(offset)+2]._x, gShape[(offset)+2]._y,
					gShape[(offset)+3]._x, gShape[(offset)+3]._y);
			}
		} 
		break;

	case GR_QUAD_STRIP:
		break;
	case GR_TRIANGLES:
		{
			int nTris = n / 3;
			for (int idx = 0; idx < nTris; idx++)
			{
				triangle(gShape[idx * 3]._x, gShape[idx * 3]._y,
					gShape[(idx * 3) + 1]._x, gShape[(idx * 3) + 1]._y,
					gShape[(idx * 3) + 2]._x, gShape[(idx * 3) + 2]._y);
			}
		} 
		break;
	case GR_TRIANGLE_STRIP:
		{
			int nTris = n - 2;
			for (int tri = 1; tri <= nTris; tri++)
			{
				triangle(gShape[tri-1]._x, gShape[tri-1]._y,
					gShape[tri]._x, gShape[tri]._y,
					gShape[tri+1]._x, gShape[tri+1]._y);
			}
		}
		break;
	case GR_TRIANGLE_FAN:
		{
			int nTris = n - 2;
			for (int tri = 1; tri <= nTris; tri++)
			{
				triangle(gShape[0]._x, gShape[0]._y,
						gShape[tri]._x, gShape[tri]._y,
						gShape[tri + 1]._x, gShape[tri + 1]._y);
			}
		}		
		break;
	}

	gShape.clear();
}


// Text Processing
void text(const char *str, const int x, const int y)
{
	int tx = x;
	int ty = y;

	// determine the box within which the string would draw
	// based on alignment
	size_t fwidth = font_t_str_width(&gfont, str);
	switch (gTextAlignX) {
	case TX_LEFT:
		tx = x;
		break;

	case TX_CENTER:
		tx = x - fwidth / 2;
		break;

	case TX_RIGHT:
		tx = x - fwidth;
		break;
	}

	switch (gTextAlignY)
	{
	case TX_TOP:
		ty = y;
		break;
	case TX_CENTER:
		ty = y - (gfont.height / 2);
		break;
	case TX_BOTTOM:
		ty = y - gfont.height;
		break;
	}
	scan_str(gpb, &gfont, tx, ty, str, fillColor.value);
}

void textAlign(const int alignX, const int alignY)
{
	gTextAlignX = alignX;
	gTextAlignY = alignY;
}

void textSize(const int size)
{
	gTextSize = size;
}

void setFont(const uint8_t *fontdata)
{
	font_t_init(&gfont, fontdata);
}

/*
	Images
*/
void image(const PImage *img, const float a, const float b, const float c, const float d)
{
	raster_rgba_blit(gpb, a, b, &img->fb);
}

PImage * loadImage(const char *filename, const char *extension)
{
	PImage *img = new PImage();

	int ret = PPM_read_binary(filename, &img->fb);

	return img;
}
