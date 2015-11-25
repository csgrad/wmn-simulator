#ifndef CLIQUE_H
#define CLIQUE_H

#include <vector>         //std::vector

#include "Link.h"

class Clique
{
  public:
  
  /* Helper Functions */
  bool equivalentClique(Clique c2);
  bool exists(int link_id);
  bool intersect(Clique c2);
  
  /* Accessors */
  int getGain(std::vector<Link> linkdata);
  int getWeight(std::vector<Link> linkdata);
  unsigned int size(){return LS.size();};
  int getLink(unsigned int l);
  
  /* Mutators */
  void addLink(int link_id);
  
  /* Display Function */
  void display();
  
  private:  
  std::vector<int> LS;
};

#endif
