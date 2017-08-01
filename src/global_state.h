#pragma once

#include "dproc_config.h"
#include "dproc_types.h"

// Global variables
extern size_t width;
extern size_t height;


 extern pb_rgba *gpb;
 extern pb_rect pixelFrame;


extern COLOR bgColor;
extern pb_rgba *bgImage;

 extern RECTMODE grectMode;
 extern RECTMODE gellipseMode;

// color setting
extern COLORMODE gColorMode;
extern float gColorMax1;
extern float gColorMax2;
extern float gColorMax3;
extern float gColorMaxA;

extern COLOR bgColor;
extern pb_rgba *bgImage;
extern COLOR strokeColor;
extern float gstrokeWeight;
extern COLOR fillColor;

// Keyboard
 extern int keyChar;
 extern int keyCode;
 extern bool isKeyPressed;

// Mouse
// Mouse
 extern int mouseX;
 extern int mouseY;
 extern int mouseButton;
 extern int mouseDelta;
 extern bool isMousePressed;
// pmouseX
// pmouseY

// Text Settings
extern font_t gfont;
extern int gTextSize;
extern int gTextAlignX;
extern int gTextAlignY;
