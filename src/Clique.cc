#ifndef CLIQUE_CC
#define CLIQUE_CC

#include "Clique.h"

/*
 * Returns the Gain for the Clique
 * Gain = sum of all link costs - largest link cost
 */
int Clique::getGain(std::vector <Link> linkdata)
{
  int gain=0;
  int largest=getWeight(linkdata);
  
  for(unsigned int c=0; c< LS.size(); c++)
  {
    int link_id = LS[c];
    gain = gain + linkdata[link_id].getRequirement();
  }
  
  return gain - largest;
}

/*
 * Returns the weight of a Clique
 * Weight = Maximal Requirement (may need to be revised)
 */
int Clique::getWeight(std::vector <Link> linkdata)
{
  int max=0;
  for(unsigned int c=0; c<LS.size(); c++)
  {
    int link_id = LS[c];
    int requirement = linkdata[link_id].getRequirement();
    if(requirement > max)
      max = requirement;
  }
  
  return max;
}

/*
 * Returns true if the two cliques are equivalent
 */
bool Clique::equivalentClique(Clique c2)
{
  /* Check for each link_id of c1 in c2 */
  for(unsigned int c=0; c< LS.size(); c++)
    if(!c2.exists(LS[c]))
      return false;
      
  for(unsigned int c=0; c< c2.size(); c++)
    if(!exists(c2.getLink(c)))
      return false;
      
  return true;
}

/* 
 * Checks if the given link_id already exists
 * within the clique
 */
bool Clique::exists(int link_id)
{
  for(unsigned int l=0;l<LS.size();l++)
    if(LS[l] == link_id)
      return true;
  return false;
}

/*
 * Returns true if c1 and c2 intersect
 */
bool Clique::intersect(Clique c2)
{
  for(unsigned int c=0;c<LS.size();c++)
    if(c2.exists(LS[c]))
      return true;
  
  for(unsigned int c=0;c<c2.size();c++)
    if(exists(c2.getLink(c)))
      return true;
  
  return false;
}

/*
 * Returns a link_id from the clique
 */
int Clique::getLink(unsigned int l)
{
  if(l > LS.size())
  {
    std::cout << "Error, you are trying to access memory outside of the clique" << std::endl;
    exit(255);
  }
  else
    return LS[l];
}

/*
 * Adds the link_id to the clique iff it is unique
 */
void Clique::addLink(int link_id)
{
  if(!exists(link_id))
    LS.push_back(link_id);
}

void Clique::display()
{
  for(unsigned int l=0; l < LS.size(); l++)
  {
    std::cout << LS[l];
    if(l != LS.size()-1)
      std::cout << ",";
  }
  std::cout << std::endl;
}

/* ---- Private Functions ---- */

#endif
