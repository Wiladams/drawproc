#pragma once

/*
	sorter

	Base class representing general sorting capability.

	A subclass MUST implement the 'step()' method.  Each step should
	perform enough of the sorting to make it visually interesting
	when the results are displayed.
*/
class sorter 
{
public:
	int * fElements;
	int fNumElements;
	char * name = nullptr;
	int stepIndex = 0;

public:
	sorter();
	sorter(const sorter &other);
	sorter(int *elems, const int nelems);
	
	virtual ~sorter();

	sorter & operator=(const sorter &other);



	virtual bool step();

	virtual void reset(int *elems, const int nelems);

	
	void exchange(const int a, const int b);


	bool compareExchange(const int a, const int b);

};


/*
	BubbleSort

	Implementation of a basic bubble sorting algorithm
*/
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
		if (stepIndex >= fNumElements-2) {
			return false;
		}
		
		int min = stepIndex;
		for (int j = stepIndex+1; j <= fNumElements-1; j++)
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
