#pragma once

#include "sortroutine.h"

/*
BubbleSort

Implementation of a basic bubble sorting algorithm
*/
class BubbleSort : public sorter
{
public:
	BubbleSort()
		: sorter("BubbleSort")
	{}

	virtual bool step()
	{
		if (stepIndex >= fNumElements) {
			return false;
		}

		for (int j = fNumElements - 1; j > stepIndex; j--)
		{
			compareExchange(j - 1, j);
		}
		stepIndex = stepIndex + 1;

		return true;
	}
};

