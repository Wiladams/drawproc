
#include "guistyle.h"
#include "drawproc.h"



void GUIStyle::SetBaseColor(const DPPIXELVAL acolor)
{
	this->baseColor = acolor;
	this->highlightColor = brighter(this->baseColor);
	this->shadowColor = darker(this->baseColor);
	this->backgroundColor = brighter(this->highlightColor);
	this->textBackgroundColor = this->baseColor;
}


/*
self:SetBaseColor(Colors.LtGray)

this->Foreground = Colors.LtGray;

this->BottomShadow = ColorUtils.darker(this->Foreground);
this->BottomShadowTopLiner = ColorUtils.brighter(this->BottomShadow);

this->TopShadow = ColorUtils.brighter(this->Foreground);

	 --Calculated
		 this->SunkenColor = this->Foreground;
	 this->RaisedColor = this->Foreground;
}
*/

void GUIStyle::DrawFrame(int x, int y, int w, int h, int style)
{	
	if (style == Sunken) {
		for (int n = 0; n < this->borderWidth; n++)
		{
			stroke(this->highlightColor);
			line(x + n, y + h - n, x + w - n, y + h - n);		// bottom shadow
			line(x + w - n, y + n, x + w - n, y + h);			// right shadow
		}

		for (int n = 0; n < this->borderWidth; n++) {
			stroke(this->shadowColor);
			line(x + n, y + n, x + w - n, y + n);	    // top edge
			line(x + n, y + n, x + n, y + h - n);	    // left edge
		}
	} else if (style == Raised) {
		for (int n = 0; n < this->borderWidth - 1; n++) {
			stroke(this->shadowColor);
			line(x + n, y + h - n, x + w - n, y + h - n);   // bottom shadow
			line(x + w - n, y + n, x + w - n, y + h);	    // right shadow
		}

		for (int n = 0; n < this->borderWidth; n++) {
			stroke(this->highlightColor);
			line(x + n, y + n, x + w - n, y + n);	    //top edge
			line(x + n, y + n, x + n, y + h - n);	    //left edge
		}
	}
}

void GUIStyle::DrawSunkenRect(const int x, const int y, const int w, const int h)
{			   
	noStroke();

	fill(this->baseColor);
	rect(x, y, w, h);

	DrawFrame(x, y, w, h, Sunken);
}

void GUIStyle::DrawRaisedRect(const int x, const int y, const int w, const int h)
{
	noStroke();
	fill(this->baseColor);
	rect(x, y, w, h);
	DrawFrame(x, y, w, h, Raised);
}


