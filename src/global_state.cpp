
#include "global_state.h"
#include "dptypes.h"


// Globals for the environment
pb_rgba *gpb;
size_t width = 0;
size_t height = 0;
DPCLIPRECT pixelFrame;
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



 DPPIXELVAL gr_foreground;	/* current foreground color */
 DPPIXELVAL gr_background;	/* current background color */
 bool 	gr_usebg;    	    /* TRUE if background drawn in pixmaps */
 int 	gr_mode = DPROP_COPY; 	    /* drawing mode */
 DPPALENTRY	gr_palette[256];    /* current palette*/
 int	gr_firstuserpalentry;/* first user-changable palette entry*/
 int 	gr_nextpalentry;    /* next available palette entry*/
 DPCOLORVAL gr_foreground_rgb;	/* current fg color in 0xAARRGGBB format for mono convblits*/
 DPCOLORVAL gr_background_rgb;	/* current background color */

 uint32_t gr_dashmask;     /* An actual bitmask of the dash values */
 uint32_t gr_dashcount;    /* The number of bits defined in the dashmask */

 int        gr_fillmode;
 DPSTIPPLE  gr_stipple;
 DPTILE     gr_tile;

 DPPOINT    gr_ts_offset;
