#ifndef LINK_CC
#define LINK_CC

#include "Link.h"

/*
 * Constructor for a link with all the required parameters
 */
Link::Link(int _first, int _second)
{
	first = _first;
	second = _second;
	requirement = 0;
}

/*
 * Displays the MRs in a link and the requirement
 */
void Link::display()
{
	std::cout << "  LS: (" << first << "," << second << ") Requirement: " << requirement << std::endl;
}

/*
 * Returns true if two links are equivalent, ie they have the same MRs in them)
 */
bool Link::equivalent(Link l2)
{
	if((first == l2.first && second == l2.second) || (first == l2.second && second == l2.first))
		return true;
	else
		return false;
}

#endif
