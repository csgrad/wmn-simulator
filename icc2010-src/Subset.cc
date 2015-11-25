// subset.cc -- Implementation of subset class
// Author:  Steve Tate
// Last Update:  September 21, 1998

#include <iostream>
#include <vector>
#include "Subset.h"

// recursive_printer(int last) prints out all subsets that have varying
// elements 1..last (last+1..n are considered fixed by this function).
// Recursively, we fix element last to be each of the two possible states
// (included and excluded), and vary the remaining last-1 elements 
// recursively.

void Subset::recursive_printer(int last)
{
    if (last == 0) {
	cout << *this << endl;
    } else {
	exclude(last);
	recursive_printer(last-1);
	include(last);
	recursive_printer(last-1);
    }
}

// Overriding the output operator -- pretty basic stuff here.

ostream &operator<<(ostream &ostr, const Subset &subset)
{
    bool first = true;
    ostr << "{";
    for (int i=0; i<subset.size; i++) {
			if (subset.flags[i]) {
	    	if (first)
					first = false;
	    	else
					ostr << ", ";
	    ostr << (i+1);
			}
    }
    ostr << "}";

    return ostr;
}

/*
int main()
{
	Subset s(10);
	s.printAllSubsets();
	
	return 0;
}*/
	

