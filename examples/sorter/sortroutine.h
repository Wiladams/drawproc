#pragma once

class sorter {
public:
	int * fElements;
	int fNumElements;

public:
	sorter(int elems[], const int nelems) :
		fElements(elems),
		fNumElements(nelems) {}

	virtual ~sorter() {}

	virtual bool step() = 0;
	virtual void reset() {}
};

class BubbleSort : public sorter
{
	int stepIndex = 0;

public:
	BubbleSort(int elems[], const int nelems) 
		: sorter(elems, nelems)
	{}

	bool compareExchange(const int a, const int b)
	{
		if (fElements[a] > fElements[b]) {
			int tmp = fElements[a];
			fElements[a] = fElements[b];
			fElements[b] = tmp;

			return true;
		}

		return false;
	}

	virtual void reset()
	{
		stepIndex = 0;
	}

	virtual bool step() 
	{
		if (stepIndex >= fNumElements) {
			return false;
		}

		//for (int idx=0; idx < fNumElements-1; idx++) {
			// next element is smaller than us
		for (int j = fNumElements - 1; j > stepIndex; j--)
		{
			compareExchange(j - 1, j);
		}
		stepIndex = stepIndex + 1;

		return true;
	}
};