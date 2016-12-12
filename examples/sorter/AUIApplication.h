#pragma once

#include "AView.h"

class AUIApplication
{

private:
	AView fMainView;

	AUIApplication(const int x, const int y, const int width, const int height);

public:
	static AUIApplication * CreateApplication(const int x, const int y, const int width, const int height);

	virtual void KeyTyped();
	virtual void KeyPressed();
	virtual void KeyReleased();

	virtual void MousePressed();
	virtual void MouseReleased();
	virtual void MouseMoved();

	virtual void Draw();

	virtual void AddChild(AView *aview);

public:
	static AUIApplication * App;

};