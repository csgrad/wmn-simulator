#ifndef LINK_H
#define LINK_H

#include <iostream>

class Link
{
	public:
		Link(int _first, int _second);
		int first, second, requirement;
		bool equivalent(Link l2);
		void display();
	private:
};

#endif
