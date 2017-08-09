#pragma once

#include "dproc_config.h"
#include "dproc_types.h"


	// Global variables
DPROC_API extern	 size_t width;
DPROC_API extern	 size_t height;


DPROC_API extern	 pb_rgba *gpb;
DPROC_API extern	 DPCLIPRECT pixelFrame;


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



DPROC_API extern DPPIXELVAL gr_foreground;	/* current foreground color */
DPROC_API extern DPPIXELVAL gr_background;	/* current background color */
DPROC_API extern bool 	gr_usebg;    	    /* TRUE if background drawn in pixmaps */
DPROC_API extern int 	gr_mode; 	    /* drawing mode */
DPROC_API extern DPPALENTRY	gr_palette[];    /* current palette*/
DPROC_API extern int	gr_firstuserpalentry;/* first user-changable palette entry*/
DPROC_API extern int 	gr_nextpalentry;    /* next available palette entry*/
DPROC_API extern DPCOLORVAL gr_foreground_rgb;	/* current fg color in 0xAARRGGBB format for mono convblits*/
DPROC_API extern DPCOLORVAL gr_background_rgb;	/* current background color */

DPROC_API extern uint32_t gr_dashmask;     /* An actual bitmask of the dash values */
DPROC_API extern uint32_t gr_dashcount;    /* The number of bits defined in the dashmask */

DPROC_API extern int        gr_fillmode;
DPROC_API extern DPSTIPPLE  gr_stipple;
DPROC_API extern DPTILE     gr_tile;

DPROC_API extern DPPOINT    gr_ts_offset;


DPROC_API extern SCREENDEVICE scrdev;
DPROC_API extern KBDDEVICE kbddev;

