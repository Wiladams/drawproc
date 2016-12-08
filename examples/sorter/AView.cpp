#include "AView.h"

bool containsPoint(pb_rect &aRect, const int x, const int y)
{
	if ((x < aRect.x) || (y < aRect.y))
		return false;

	if ((x >= aRect.x + aRect.width) || (y >= aRect.y + aRect.height))
		return false;

	return true;
}

void AView::AddChild(AView *child)
{
	fChildren.push_back(child);
}

// Find children in Z order (last to be added is closest),
// based on a location.
// Ideally children would be held in a quad-tree instead
// of a simple list for faster location searches.
int AView::FindChildrenAtPoint(const int x, const int y, std::vector<AView *> &childList)
{
	int numFound = 0;

	for (int idx = 0; idx < fChildren.size(); idx++) {
		if (containsPoint(fChildren[idx]->fFrame, x, y)) {
			childList.push_back(fChildren[idx]);
			numFound = numFound + 1;
		}
	}

	return numFound;
}

void AView::drawBackground()
{
	background(pLightGray);
}

void AView::drawChildren()
{
	for (int idx = 0; idx < fChildren.size(); idx++)
		fChildren[idx]->draw();
}

void AView::drawForeground()
{}

void AView::draw()
{
	drawBackground();
	drawChildren();
	drawForeground();
}

void AView::KeyPressed()
{
	// go through children seeing if one of them
	// will handle the keypress
	std::vector<AView *> childList;
	int numFound = FindChildrenAtPoint(mouseX, mouseY, childList);

	if (numFound > 0) {
		childList.at(0)->KeyPressed();
		return;
	}

	OnKeyPressed();
}

void AView::KeyReleased()
{
	// go through children seeing if one of them
	// will handle the keypress
	std::vector<AView *> childList;
	int numFound = FindChildrenAtPoint(mouseX, mouseY, childList);

	if (numFound > 0) {
		childList.at(0)->KeyReleased();
		return;
	}

	OnKeyReleased();
}

void AView::KeyTyped()
{
	// go through children seeing if one of them
	// will handle the keypress
	std::vector<AView *> childList;
	int numFound = FindChildrenAtPoint(mouseX, mouseY, childList);

	if (numFound > 0) {
		childList.at(0)->KeyTyped();
		return;
	}

	OnKeyTyped();
}

bool AView::OnKeyPressed()
{
	return false;
}

bool AView::OnKeyTyped()
{
	return false;
}

bool AView::OnKeyReleased()
{
	return false;
}

// Mouse Handling
void AView::MousePressed()
{
	// go through children seeing if one of them
	// will handle the activity
	std::vector<AView *> childList;
	int numFound = FindChildrenAtPoint(mouseX, mouseY, childList);

	if (numFound > 0) {
		childList.at(0)->MousePressed();
		return;
	}

	OnMousePressed();
}

void AView::MouseReleased()
{
	// go through children seeing if one of them
	// will handle the activity
	std::vector<AView *> childList;
	int numFound = FindChildrenAtPoint(mouseX, mouseY, childList);

	if (numFound > 0) {
		childList.at(0)->MouseReleased();
		return;
	}

	OnMouseReleased();
}

void AView::MouseMoved()
{
	// go through children seeing if one of them
	// will handle the activity
	std::vector<AView *> childList;
	int numFound = FindChildrenAtPoint(mouseX, mouseY, childList);

	if (numFound < 1) {
		// if we've moved off a previous child, then 
		// let it know that
		if (fLastHoverChild) {
			fLastHoverChild->OnMouseExited();
			fLastHoverChild = nullptr;
		}

		// call our move handler
		OnMouseMoved();
		return;
	}

	AView *newHoverChild = childList.at(0);
	if (fLastHoverChild == nullptr) {
		fLastHoverChild = newHoverChild;
		fLastHoverChild->OnMouseEntered();

		return;
	}
	else {
		if (fLastHoverChild != newHoverChild) {
			fLastHoverChild->OnMouseExited();
		}
	}

	if (fLastHoverChild != newHoverChild) {
		fLastHoverChild = newHoverChild;
		fLastHoverChild->OnMouseEntered();
	}
	newHoverChild->MouseMoved();
}

bool AView::OnMousePressed()
{
	return false;
}

bool AView::OnMouseReleased()
{
	return false;
}

bool AView::OnMouseMoved()
{
	// find out which client we're currently hovering over
	// if it's different than the last one, and the last one
	// was NOT null, then send the last one 'mouseExicted',
	// send the current one 'mouseEntered'.

	return false;
}

bool AView::OnMouseEntered()
{
	return false;
}

bool AView::OnMouseExited()
{
	return false;
}
