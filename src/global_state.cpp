
#include "global_state.h"

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
int mouseX = 0;
int mouseY = 0;
bool isMousePressed = false;
int mouseButton = 0;
int mouseDelta = 0;
