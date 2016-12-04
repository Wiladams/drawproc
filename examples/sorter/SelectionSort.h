#pragma once

#include "sortroutine.h"


class SelectionSort : public sorter
{
public:
	SelectionSort()
		: sorter("SelectionSort")
	{}


	virtual bool step()
	{
		if (stepIndex >= fNumElements - 2) {
			return false;
		}

		int min = stepIndex;
		for (int j = stepIndex + 1; j <= fNumElements - 1; j++)
		{
			if (fElements[j] < fElements[min]) {
				min = j;
			}
		}
		exchange(stepIndex, min);

		stepIndex = stepIndex + 1;

		return true;
	}
};
