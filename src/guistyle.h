#pragma once

#include "drawproc.h"

enum FrameStyle {
	Sunken = 0x01,
	Raised = 0x02
};

// Some color manipulation routines
inline DPPIXELVAL darker(const DPPIXELVAL acolor, const float factor = 0.60)
{
	return RGBA(REDVALUE(acolor) *factor, GREENVALUE(acolor) * factor, BLUEVALUE(acolor) * factor, ALPHAVALUE(acolor));
}

inline DPPIXELVAL brighter(const DPPIXELVAL acolor, const float factor = 0.80)
{
	return RGBA(
		CLAMP(REDVALUE(acolor) *(1.0 / factor), 0, 255),
		CLAMP(GREENVALUE(acolor) *(1.0 / factor), 0, 255),
		CLAMP(BLUEVALUE(acolor) *(1.0 / factor), 0, 255),
		ALPHAVALUE(acolor));
}

struct GUIStyle {
	DPPIXELVAL baseColor;
	DPPIXELVAL highlightColor;
	DPPIXELVAL shadowColor;
	DPPIXELVAL backgroundColor;
	DPPIXELVAL textBackgroundColor;
	int borderWidth;
	int padding;

	GUIStyle() :
		borderWidth(2), 
		padding(2){}



	void SetBaseColor(const DPPIXELVAL color);
	void DrawFrame(int x, int y, int w, int h, int style);
	void DrawSunkenRect(const int x, const int y, const int w, const int h);
	void DrawRaisedRect(const int x, const int y, const int w, const int h);

};
