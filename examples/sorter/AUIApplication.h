#pragma once

#include "AView.h"

class AUIApplication : public AView
{

private:
	AUIApplication(const int x, const int y, const int width, const int height);

public:
	static AUIApplication * CreateApplication(const int x, const int y, const int width, const int height);


public:
	static AUIApplication * App;

};