#include "AUIApplication.h"

AUIApplication::AUIApplication()
{
	AUIApplication::App = nullptr;
}

AUIApplication::AUIApplication(const int x, const int y, const int width, const int height)
	:fMainView(x, y, width, height)
{}

/*
	We use a factory constructor for the Application object
	because it is a singleton.  Only one can exist within 
	the context of the running application.
*/
AUIApplication * AUIApplication::CreateApplication(const int x, const int y, const int width, const int height)
{
	if (AUIApplication::App != nullptr) {
		return AUIApplication::App;
	}

	AUIApplication::App = new AUIApplication(x, y, width, height);

	return App;
}

void AUIApplication::KeyTyped()
{
	fMainView.KeyTyped();
}

void AUIApplication::KeyPressed()
{
	fMainView.KeyPressed();
}

void AUIApplication::KeyReleased()
{
	fMainView.KeyReleased();
}

void AUIApplication::MouseReleased()
{
	fMainView.MouseReleased();
}

void AUIApplication::MousePressed()
{
	fMainView.MousePressed();
}

void AUIApplication::MouseMoved()
{
	fMainView.MouseMoved();
}

/*
	Implementation of global functions for the drawproc
	environment.
*/
// Keyboard Handling
void keyTyped()
{
	AUIApplication::App->KeyTyped();
}

void keyPressed()
{
	AUIApplication::App->KeyPressed();
}

void keyReleased()
{
	AUIApplication::App->KeyReleased();
}

// Mouse Handling
void mouseClicked()
{
	AUIApplication::App->MouseClicked)();
}

void mouseMoved()
{
	AUIApplication::App->MouseMoved();
}

void mousePressed()
{
	AUIApplication::App->MousePressed();
}

void mouseReleased()
{
	AUIApplication::App->MouseReleased();
}
