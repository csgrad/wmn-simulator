// Subset.h -- Example of recursive printing of subsets
// Author:  Steve Tate
// Last Update:  September 21, 1998

#ifndef SUBSET_H
#define SUBSET_H

#include <iostream>
using namespace std;

/**
 * For subsets of integer values from 1 to n.
 *
 * Class Subset is for maintaining subsets of the set {1,2,...,n}, where
 * n is a value that is fixed when the Subset is allocated (by the
 * constructor).  Items can be include and excluded, and you can print
 * out all possible subsets of {1,2,...,n}.
 *
 * The output operator << has been overridden, so subsets can be
 * printed out using the regular iostream operations.
 */

class Subset {
    int size;
    bool *flags;

    friend ostream &operator<<(ostream &ostr, const Subset &subset);

    // recursive_printer() is a private "helper method" used to print all 
    // subsets.

    void recursive_printer(int last);

public:
    /** 
     * Constructor -- Sets the size of the "base set" and initializes
     * the subset to empty.
     *
     * @param n The size of the "base set" {1,2,...,n} */
    Subset(int n) {
	if ((flags = new bool[n]) == 0) {
	    cerr << "Couldn't allocate memory!\n";
	    exit(1);
	}

	for (int i=0; i<n; i++)
	    flags[i] = false;

	size = n;
    };

    /**
     * Include item index in the subset.  If it is already in the subset
     * then nothing changes.
     *
     * @param index The element to include in the subset.
     */
    void include(int index) {
	if ((index <= 0) || (index > size)) {
	    cerr << "Invalid index sent to Subset::include!\n";
	} else {
	    flags[index-1] = true;
	}
    };

    /**
     * Exclude item index in the subset.  If it is not in the subset
     * originally, then nothing changes.
     *
     * @param index The element to exclude from the subset.
     */
    void exclude(int index) {
	if ((index <= 0) || (index > size)) {
	    cerr << "Invalid index sent to Subset::include!\n";
	} else {
	    flags[index-1] = false;
	}
    };

    /**
     * Print out all possible subsets of the base set.
     */
    void printAllSubsets() {
	recursive_printer(size);
    };
};

ostream &operator<<(ostream &ostr, const Subset &subset);

#endif
