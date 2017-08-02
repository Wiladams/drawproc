#pragma once

#include "dproc_config.h"
#include "dproc_types.h"


	// Global variables
DPROC_API extern	 size_t width;
DPROC_API extern	 size_t height;


DPROC_API extern	 pb_rgba *gpb;
DPROC_API extern	 pb_rect pixelFrame;


DPROC_API extern	 COLOR bgColor;
DPROC_API extern	 pb_rgba *bgImage;

DPROC_API extern	 RECTMODE grectMode;
DPROC_API extern	 RECTMODE gellipseMode;

	// color setting
DPROC_API extern	 COLORMODE gColorMode;
DPROC_API extern	 float gColorMax1;
DPROC_API extern	 float gColorMax2;
DPROC_API extern	 float gColorMax3;
DPROC_API extern	 float gColorMaxA;

DPROC_API extern	COLOR bgColor;
DPROC_API extern	COLOR strokeColor;
DPROC_API extern	 float gstrokeWeight;
DPROC_API extern	 COLOR fillColor;
DPROC_API extern	 pb_rgba *bgImage;

	// Keyboard
DPROC_API extern	 int keyChar;
DPROC_API extern	 int keyCode;
DPROC_API extern	 bool isKeyPressed;

	// Mouse
	// Mouse
DPROC_API extern	 int mouseX;
DPROC_API extern	 int mouseY;
DPROC_API extern	 int mouseButton;
DPROC_API extern	 int mouseDelta;
DPROC_API extern	 bool isMousePressed;
	// pmouseX
	// pmouseY

	// Text Settings
DPROC_API extern	 font_t gfont;
DPROC_API extern	 int gTextSize;
DPROC_API extern	 int gTextAlignX;
DPROC_API extern	 int gTextAlignY;
