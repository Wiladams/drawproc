/*
test_keyboard

Do some simple mouse and keyboard tracking

*/
#include "drawproc.h"
#include "guistyle.h"
#include <stdio.h>




/*
Key coordinates

*/
struct keyloc {
	pb_rect loc;
	int vkey;

	bool contains(const coord x, const coord y)
	{
		coord left = loc.x - loc.width / 2;
		coord right = loc.x + loc.width / 2;
		coord top = loc.y - loc.height / 2;
		coord bottom = loc.y + loc.height / 2;

		if (x<left || x>right) {
			return false;
		}

		if (y<top || y>bottom) {
			return false;
		}

		return true;
	}
};

static GUIStyle styler;
static const int gMaxMode = 3;
static int gMode = 0;
static const int gridSize = 8;
static struct keyloc hoverKey;

static bool gShowKeyRect = false;
static bool showGrid = false;
static bool gShowCrosshair = false;
static bool gShowKeyOutlines = false;

PImage *kbImage;
pb_rgba fb;
pb_rect keyRect = { 0, 0, 34, 34 };


struct keyloc locations[] = {
	// Row 1 - function keys
	{ { 30, 28, 32,34 }, KC_ESCAPE },
	// F1 cluster
	{ { 96, 28, 32, 34 }, KC_F1 },
	{ { 134, 28, 32, 34 }, KC_F2 },
	{ { 172, 28, 32, 34 }, KC_F3 },
	{ { 210, 28, 32, 34 }, KC_F4 },
	// F5 cluster
	{ { 270, 28, 32, 34 }, KC_F5 },
	{ { 308, 28, 32, 34 }, KC_F6 },
	{ { 346, 28, 32, 34 }, KC_F7 },
	{ { 384, 28, 32, 34 }, KC_F8 },
	// F9 cluster
	{ { 445, 28, 32, 34 }, KC_F9 },
	{ { 483, 28, 32, 34 }, KC_F10 },
	{ { 521, 28, 32, 34 }, KC_F11 },
	{ { 559, 28, 32, 34 }, KC_F12 },
	// Print Screen cluster
	{ { 618, 28, 32, 34 }, KC_PRINT },
	{ { 656, 28, 32, 34 }, KC_SCROLL },
	{ { 694, 28, 32, 34 }, KC_PAUSE },


	// Row 2
	{ { 30, 100, 32, 32 }, KC_OEM_3 },	// ~
	{ { 68, 100, 32, 32 }, KC_1 },
	{ { 106, 100, 32, 32 }, KC_2 },
	{ { 144, 100, 32, 32 }, KC_3 },
	{ { 182, 100, 32, 32 }, KC_4 },
	{ { 220, 100, 32, 32 }, KC_5 },
	{ { 258, 100, 32, 32 }, KC_6 },
	{ { 296, 100, 32, 32 }, KC_7 },
	{ { 334, 100, 32, 32 }, KC_8 },
	{ { 370, 100, 32, 32 }, KC_9 },
	{ { 408, 100, 32, 32 }, KC_0 },
	{ { 446, 100, 32, 32 }, KC_OEM_MINUS },
	{ { 486, 100, 32, 32 }, KC_OEM_PLUS },
	{ { 542, 100, 66, 32 }, KC_BACK },
	// INSERT cluster
	{ { 618, 100, 32, 34 }, KC_INSERT },
	{ { 656, 100, 32, 34 }, KC_HOME },
	{ { 694, 100, 32, 34 }, KC_PRIOR },		// Page Up
	// Num Lock cluster
	{ { 754, 100, 32, 34 }, KC_NUMLOCK },
	{ { 792, 100, 32, 34 }, KC_DIVIDE },
	{ { 830, 100, 32, 34 }, KC_MULTIPLY },
	{ { 868, 100, 32, 34 }, KC_SUBTRACT },

	// Row 3 - QWERTY
	{ { 38, 140, 50, 32 }, KC_TAB },
	{ { 86, 140, 32, 32 }, KC_Q },
	{ { 124, 140, 32, 32 }, KC_W },
	{ { 162, 140, 32, 32 }, KC_E },
	{ { 200, 140, 32, 32 }, KC_R },
	{ { 238, 140, 32, 32 }, KC_T },
	{ { 276, 140, 32, 32 }, KC_Y },
	{ { 314, 140, 32, 32 }, KC_U },
	{ { 352, 140, 32, 32 }, KC_I },
	{ { 390, 140, 32, 32 }, KC_O },
	{ { 428, 140, 32, 32 }, KC_P },
	{ { 466, 140, 32, 32 }, KC_OEM_4 },	// [{
	{ { 504, 140, 32, 32 }, KC_OEM_6 },	// ]}
	{ { 550, 140, 50, 32 }, KC_OEM_5 },	// \|
	// Delete cluster
	{ { 618, 140, 32, 32 }, KC_DELETE },
	{ { 656, 140, 32, 32 }, KC_END },
	{ { 694, 140, 32, 32 }, KC_NEXT },	// Page Down

	{ { 754, 140, 32, 32 }, KC_NUMPAD7 },	// or Home
	{ { 792, 140, 32, 32 }, KC_NUMPAD8 },	// or Up
	{ { 830, 140, 32, 32 }, KC_NUMPAD9 },	// or PgUp

	// Row - Keypad Plus
	{ { 868, 160, 32, 70 }, KC_ADD },

	// Row 4 - ASDF
	{ { 43, 179, 60, 32 }, KC_CAPITAL },
	{ { 95, 179, 32, 32 }, KC_A },
	{ { 133, 179, 32, 32 }, KC_S },
	{ { 171, 179, 32, 32 }, KC_D },
	{ { 210, 179, 32, 32 }, KC_F },
	{ { 248, 179, 32, 32 }, KC_G },
	{ { 286, 179, 32, 32 }, KC_H },
	{ { 324, 179, 32, 32 }, KC_J },
	{ { 362, 179, 32, 32 }, KC_K },
	{ { 400, 179, 32, 32 }, KC_L },
	{ { 438, 179, 32, 32 }, KC_OEM_1 },	// ;:
	{ { 476, 179, 32, 32 }, KC_OEM_7 },	// '"
	{ { 536, 179, 76, 32 }, KC_RETURN },
	// Keypad 4 cluster
	{ { 754, 179, 32, 32 }, KC_NUMPAD4 },	// or LEFT
	{ { 792, 179, 32, 32 }, KC_NUMPAD5 },
	{ { 830, 179, 32, 32 }, KC_NUMPAD6 },	// or RIGHT

	// Row 5 - ZXCV
	{ { 56, 218, 86, 32 }, KC_LSHIFT },
	{ { 122, 218, 32, 32 }, KC_Z },
	{ { 160, 218, 32, 32 }, KC_X },
	{ { 198, 218, 32, 32 }, KC_C },
	{ { 237, 218, 32, 32 }, KC_V },
	{ { 275, 218, 32, 32 }, KC_B },
	{ { 313, 218, 32, 32 }, KC_N },
	{ { 351, 218, 32, 32 }, KC_M },
	{ { 389, 218, 32, 32 }, KC_OEM_COMMA },
	{ { 428, 218, 32, 32 }, KC_OEM_PERIOD },
	{ { 467, 218, 32, 32 }, KC_OEM_2 },		// /?
	{ { 532, 218, 86, 32 }, KC_RSHIFT },
	{ { 656, 218, 32, 32 }, KC_UP },
	// Keypad 1 cluster
	{ { 754, 218, 32, 32 }, KC_NUMPAD1 },
	{ { 792, 218, 32, 32 }, KC_NUMPAD2 },
	{ { 830, 218, 32, 32 }, KC_NUMPAD3 },

	// Row - Keypad ENTER
	{ { 868, 238, 32, 72 }, KC_OEM_NEC_EQUAL },

	// Row 5 - SPACE
	{ { 38, 257, 50, 31 }, KC_CONTROL },		// LCONTROL
	{ { 91, 257, 44, 31 }, KC_LWIN },
	{ { 140, 257, 44, 31 }, KC_SPACE },		// LALT
	{ { 270, 257, 202, 31 }, KC_SPACE },	// SPACE BAR
	{ { 398, 257, 44, 31 }, KC_SPACE },		// RALT
	{ { 448, 257, 44, 31 }, KC_RWIN },
	{ { 496, 257, 44, 31 }, KC_MENU },		// RMENU
	{ { 550, 257, 50, 31 }, KC_CONTROL },		// RCONTROL

	{ { 618, 257, 32, 31 }, KC_LEFT },
	{ { 656, 257, 32, 31 }, KC_DOWN },
	{ { 694, 257, 32, 31 }, KC_RIGHT },
	
	{ { 773, 257, 70, 31 }, KC_NUMPAD0 },
	{ { 830, 257, 32, 31 }, KC_DECIMAL },
};

static int nKeyLocs = sizeof(locations) / sizeof(locations[0]);



// traverse key locations table looking for a match
// for the specified location
bool findKey(const coord x, const coord y, struct keyloc &loc)
{
	loc.vkey = 0;

	for (int idx = 0; idx < nKeyLocs; idx++) {
		if (locations[idx].contains(x, y)) {
			loc = locations[idx];
			return true;
		}
	}

	return false;
}

void  mousePressed()
{
	gMode++;
	if (gMode >= gMaxMode) {
		gMode = 0;
	}
}

void  mouseMoved()
{
	keyRect.x = mouseX - keyRect.width / 2;
	keyRect.y = mouseY - keyRect.height / 2;

	findKey(mouseX, mouseY, hoverKey);
}

void  keyReleased()
{
	switch (keyCode)
	{
	case KC_SPACE:
		write_PPM_binary("test_keyboard.ppm", gpb);
		break;

	case KC_F1:
		showGrid = !showGrid;
		break;
	case KC_F2:
		gShowKeyRect = !gShowKeyRect;
		break;
	case KC_F3:
		gShowCrosshair = !gShowCrosshair;
		break;
	case KC_F4:
		gShowKeyOutlines = !gShowKeyOutlines;
		break;

	case KC_RIGHT: // increase width of keyRect
		keyRect.width += 1;
		break;

	case KC_LEFT:
		keyRect.width -= 1;
		if (keyRect.width < 4) keyRect.width = 4;
		break;
	case KC_UP:
		keyRect.height += 1;
		break;
	case KC_DOWN:
		keyRect.height -= 1;
		if (keyRect.height < 4) keyRect.height = 4;
		break;
	}

	keyRect.x = mouseX - keyRect.width / 2;
	keyRect.y = mouseY - keyRect.height / 2;

}

void drawKeys()
{
	static int nkeylocs = sizeof(locations) / sizeof(locations[0]);

	if (!gShowKeyOutlines) {
		return;
	}

	rectMode(CENTER);
	stroke(pRed);
	noFill();
	for (int idx = 0; idx < nkeylocs; idx++){
		if (locations[idx].loc.x != 0) {
			rect(locations[idx].loc.x, locations[idx].loc.y, locations[idx].loc.width, locations[idx].loc.height);
		}
	}
}

void drawHoverKey()
{
	if (hoverKey.vkey == 0) {
		return;
	}

	rectMode(CENTER);
	stroke(pYellow);
	noFill();
	rect(hoverKey.loc.x, hoverKey.loc.y, hoverKey.loc.width, hoverKey.loc.height);
}

void drawCrossHairs()
{
	if (!gShowCrosshair) {
		return;
	}

	stroke(84);

	// horizontal line
	line(0, mouseY, width - 1, mouseY);
	line(mouseX, 0, mouseX, height - 1);
}

void drawGrid()
{
	if (!showGrid) {
		return;
	}

	stroke(132, 86);
	for (int x = 0; x < width; x++)
	{
		if (x % gridSize == 0) {
			line(x, 0, x, height - 1);
		}
	}

	for (int y = 0; y < height; y++)
	{
		if (y % gridSize == 0) {
			line(0, y, width-1, y);
		}
	}
}
// Draw information about the mouse
// location, buttons pressed, etc
void drawMouseInfo()
{
	// draw a white banner across the bottom
	rectMode(CORNER);
	noStroke();
	fill(pWhite);
	rect(0, height - 1 - 34, width, 34);

	if (gShowKeyRect) {
		// draw the key rectangle
		fill(COLOR(255, 238, 200, 180));
		stroke(pDarkGray);
		rectMode(CORNER);
		rect(keyRect.x, keyRect.y, keyRect.width, keyRect.height);
	}

	char infobuff[512] = { '\0' };
	sprintf_s(infobuff, sizeof(infobuff), "Mouse X: %3d Y: %3d  Key Code: 0x%x", mouseX, mouseY, keyCode);
	fill(pBlack);
	textAlign(TX_LEFT, TX_TOP);
	text(infobuff, 0, height-34);

}

void draw()
{
	background(pLightGray);
	image(kbImage, 0, 0);

	drawCrossHairs();
	drawGrid();
	drawKeys();
	drawHoverKey();

	drawMouseInfo();
}

void setup()
{
	// Adjust this path to match where the keyboard image file is
	kbImage = loadImage("c:/repos/graphicc/Test/windows-keyboard-60-keys.ppm", nullptr);
	
	size(kbImage->fb.frame.width, kbImage->fb.frame.height+4+30);
	background(pLightGray);

	// select verdana font
	setFont(verdana17);
}


