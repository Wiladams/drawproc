#pragma once

#include "AView.h"

class AUIApplication
{

private:
	AView fMainView;

	AUIApplication(const int x, const int y, const int width, const int height);

public:
	static AUIApplication * CreateApplication(const int x, const int y, const int width, const int height);

	// Input handling
	virtual void KeyTyped();
	virtual void KeyPressed();
	virtual void KeyReleased();

	virtual void MousePressed();
	virtual void MouseReleased();
	virtual void MouseMoved();

	// Drawing
	virtual void Draw();

	// UI Element Hierarchy
	virtual void AddChild(AView *aview);

public:
	static AUIApplication * App;

};