#pragma once

#include "drawproc.h"

enum FrameStyle {
	Sunken = 0x01,
	Raised = 0x02
};

// Some color manipulation routines
inline COLOR darker(const COLOR acolor, const float factor = 0.60)
{
	return COLOR(acolor.r *factor, acolor.g * factor, acolor.b * factor, acolor.a);
}

inline COLOR brighter(const COLOR acolor, const float factor = 0.80)
{
	return COLOR(
		CLAMP(acolor.r *(1.0 / factor), 0, 255), 
		CLAMP(acolor.g *(1.0 / factor), 0, 255), 
		CLAMP(acolor.b *(1.0 / factor), 0, 255), 
		acolor.a);
}

struct GUIStyle {
	COLOR baseColor;
	COLOR highlightColor;
	COLOR shadowColor;
	COLOR backgroundColor;
	COLOR textBackgroundColor;
	int borderWidth;
	int padding;

	GUIStyle() :
		borderWidth(2), 
		padding(2){}



	void SetBaseColor(const COLOR color);
	void DrawFrame(int x, int y, int w, int h, int style);
	void DrawSunkenRect(const int x, const int y, const int w, const int h);
	void DrawRaisedRect(const int x, const int y, const int w, const int h);

};
