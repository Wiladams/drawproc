#pragma once

#include "drawproc.h"
#include <vector>

class AView {
public:
	// Attributes of the view
	pb_rect fFrame;
	COLOR fBackgroundColor;

	// current state of view
	bool fActive;
	std::vector<AView *> fChildren;

	// child cursor management
	AView *fLastActiveChild = nullptr;
	AView *fLastHoverChild = nullptr;

	AView(const int x, const int y, const int pWidth, const int pHeight) 
		: fFrame(x, y, pWidth, pHeight)
	{}

	// Hierarchy management
	void AddChild(AView *child);
	int FindChildrenAtPoint(const int x, const int y, std::vector<AView *> &childList);

	// Drawing
	virtual void drawBackground();
	virtual void drawForeground();
	virtual void drawChildren();	
	virtual void draw();


	// Keyboard Handling
	virtual void KeyPressed();
	virtual void KeyReleased();
	virtual void KeyTyped();

	virtual bool OnKeyPressed();
	virtual bool OnKeyReleased();
	virtual bool OnKeyTyped();

	// Mouse Handling
	virtual void MousePressed();
	virtual void MouseReleased();
	virtual void MouseMoved();

	virtual bool OnMousePressed();
	virtual bool OnMouseReleased();
	virtual bool OnMouseMoved();

	virtual bool OnMouseEntered();
	virtual bool OnMouseExited();
};