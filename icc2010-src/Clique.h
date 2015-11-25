#ifndef CLIQUE_H
#define CLIQUE_H 

#include <vector>
#include "Link.h"

class Clique
{
	public:
		void display();
		bool equivalent(Clique c2);
		bool intersect(Clique c2);
		int getGain();
		int getWeight();
		std::vector <Clique> decompose();
		std::vector<Clique> recursive_decompose(int last);
		void exclude(int i);
		void include(int i);
		std::vector <Link> links;
		std::vector <bool> status;
};

#endif
