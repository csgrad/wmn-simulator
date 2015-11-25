#ifndef LINK_H
#define LINK_H

#include <iostream>       //std::cout/cin, srand48, drand48

class Link
{
  public:
  /* Constructors */
  Link(){id=-1;source_id=-1; destination_id=-1; requirement=0;};
  Link(int i, int s, int d){id=i;source_id = s;destination_id = d; requirement=0;};
  
  /* Functions */
  bool interfere(Link LS);
  
  /* Accessors */
  int getID(){return id;};
  int getSource(){return source_id;};
  int getDestination(){return destination_id;};
  int getRequirement(){return requirement;};
  
  /* Mutators */
  void setRequirement(int r){requirement = r;};
  
  /* Display */
  void display();
  
  private:
  int id, source_id, destination_id, requirement;
};

#include "Link.cc"

#endif
