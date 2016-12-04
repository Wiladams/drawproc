
#include "sortroutine.h"
/*
sorter

Base class representing general sorting capability.

A subclass MUST implement the 'step()' method.  Each step should
perform enough of the sorting to make it visually interesting
when the results are displayed.
*/


sorter::sorter(const std::string &name) :
	fElements(nullptr),
	fNumElements(0),
	name(name),
	stepIndex(0)
{
}

/*
sorter::sorter(const sorter &other) 
{
	fElements = other.fElements;
	fNumElements = other.fNumElements;
	name = other.name;
	stepIndex = other.stepIndex;
}
*/

sorter::~sorter() 
{
}

/*
sorter & sorter::operator=(const sorter &other)
{
	fElements = other.fElements;
	fNumElements = other.fNumElements;
	name = other.name;
	stepIndex = other.stepIndex;

	return *this;
}
*/

bool sorter::step() 
{ 
	stepIndex = stepIndex + 1; 
	return true;
}

void sorter::sort(int *elems, const int nelems)
{
	fElements = elems;
	fNumElements = nelems;
	stepIndex = 0;
}

void sorter::exchange(const int a, const int b)
{
		int tmp = fElements[a];
		fElements[a] = fElements[b];
		fElements[b] = tmp;
}

bool sorter::compareExchange(const int a, const int b)
{
	if (fElements[a] > fElements[b]) {
		exchange(a, b);

		return true;
	}

	return false;
}



/*
BubbleSort

Implementation of a basic bubble sorting algorithm
*/
/*
class BubbleSort : public sorter
{
public:
	BubbleSort(int *elems, const int nelems)
		: sorter(elems, nelems)
	{
		name = "BubbleSort";
	}

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


class SelectionSort : public sorter
{
public:
	SelectionSort(int *elems, const int nelems)
		: sorter(elems, nelems)
	{
		name = "SelectionSort";
	}


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
*/
