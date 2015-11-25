#ifndef CLIQUE_CC
#define CLIQUE_CC

#include "Clique.h"

/*
 * Returns a vector of cliques which are one link smaller than the original
 *
std::vector<Clique> Clique::decompose()
{
	//std::cout << "  decomposing..." << std::endl;
	//display();
	std::vector <Clique>subCliques;
	
	for(int x=0;x<(int)links.size();x++)
	{
		Clique newClique;
		for(int y=0;y<(int)links.size();y++)
		{
			if(y!=x)
			{
				newClique.links.push_back(links[y]);
			}
		}
		subCliques.push_back(newClique);
	}
	
	for(int x=0;x<(int)subCliques.size();x++)
	{
			if(subCliques[x].links.size() > 2)
			{
				std::vector <Clique> smallerCliques = subCliques[x].decompose();
				subCliques.insert(subCliques.end(), smallerCliques.begin(), smallerCliques.end());
			}
	}
	
	return subCliques;
}
*/


std::vector<Clique> Clique::decompose()
{
	status.clear();
	
	for(int x=0; x<(int)links.size(); x++)
	{
		status.push_back(false);
	}
	
	return recursive_decompose(links.size());
}

void Clique::exclude(int i)
{
	if(i>0 && i < (int)status.size())
		status[i-1] = false;
}

void Clique::include(int i)
{
	if(i>0 && i < (int)status.size())
		status[i-1] = true;
}

std::vector<Clique> Clique::recursive_decompose(int last)
{
	std::vector<Clique> subset;
	
	if(last == 0)
	{
		Clique c;
		for(int i=0; i<(int)status.size(); i++)
		{
			if(status[i])
				c.links.push_back(links[i]);
		}
		subset.push_back(c);
	}
	else
	{
		std::vector<Clique> subsets;
		exclude(last);
		subsets = recursive_decompose(last-1);
		subset.insert(subset.begin(), subsets.begin(), subsets.end());
		include(last);
		subsets = recursive_decompose(last-1);
		subset.insert(subset.begin(), subsets.begin(), subsets.end());
	}
	
	return subset;
}

/*
 * Displays a clique
 */
void Clique::display()
{
	std::cout << "  Clique: " << std::endl;
	for(int x=0; x<(int)links.size(); x++)
	{
		links[x].display();
	}
}

/*
 * Returns true if clique1 and clique2 are equivalent, ie have all of the 
 * same links
 */
bool Clique::equivalent(Clique c2)
{
	//search c2 for all links in c1
	for(int x=0;x<(int)links.size();x++)
	{
		bool found = false;
		for(int y=0;y<(int)c2.links.size();y++)
		{
			if(links[x].equivalent(c2.links[y]))
				found = true;
		}
		if(!found)
			return false;
	}
	
	//search c1 for all links in c2
	for(int x=0;x<(int)c2.links.size();x++)
	{
		bool found = false;
		for(int y=0;y<(int)links.size();y++)
		{
			if(c2.links[x].equivalent(links[y]))
				found = true;
		}
		if(!found)
			return false;
	}
	
	return true;
}

/*
 * Returns true if clique1 and clique2 intersection, ie they share at
 * least one common link
 */
bool Clique::intersect(Clique c2)
{
	//search c2 for all links in c1
	for(int x=0;x<(int)links.size();x++)
	{
		for(int y=0;y<(int)c2.links.size();y++)
		{
			if(links[x].equivalent(c2.links[y]))
				return true;
		}
	}
	
	//search c1 for all links in c2
	for(int x=0;x<(int)c2.links.size();x++)
	{
		for(int y=0;y<(int)links.size();y++)
		{
			if(c2.links[x].equivalent(links[y]))
				return true;
		}
	}
	
	return false;
}

/*
 * Returns the gain for a given clique
 */
int Clique::getGain()
{
	int gain=0; int max=0;
	for(int x=0;x<(int)links.size();x++)
	{
		int weight = links[x].requirement;
		if(weight > max)
			max = weight;
		gain += weight;
	}
	return gain - max;
}

/*
 * Returns the maximum weight for a given clique
 */
int Clique::getWeight()
{
	int max=0;
	for(int x=0;x<(int)links.size();x++)
	{
		int weight = links[x].requirement;
		if(weight > max)
			max = weight;
	}
	return max;
}
#endif
