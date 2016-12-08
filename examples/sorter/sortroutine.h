#pragma once

#include <string>

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
	const std::string name;
	int stepIndex = 0;

public:
	sorter(const std::string &name);
	
	virtual ~sorter();

	virtual bool step();
	virtual void sort(int *elems, const int nelems);
	
	void exchange(const int a, const int b);
	bool compareExchange(const int a, const int b);
};
