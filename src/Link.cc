#ifndef LINK_CC
#define LINK_CC

#include "Link.h"

bool Link::interfere(Link LS)
{
  if(destination_id == LS.getDestination() || source_id == LS.getSource() || source_id == LS.getDestination() || destination_id == LS.getSource())
    return true;
  else
    return false;
}

void Link::display()
{
  std::cout << "L: " << id << "\tS: " << source_id << "\tD: " << destination_id << "\t Req: " << getRequirement() << std::endl;
}

#endif
