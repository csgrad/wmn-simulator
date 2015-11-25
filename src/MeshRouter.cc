/*
 * C++ Discrete Event Simulation
 * for Wireless Mesh Networks
 * by Jason Ernst
 * University of Guelph, ON, Canada
 * Revision 2 September 2008
 * 
 * MeshRouter Source Code
 */

#ifndef MESHROUTER_CC
#define MESHROUTER_CC

#include "MeshRouter.h"
#include <cstdlib>

void MeshRouter::display()
{
  std::cout << "MR: " << id << "\tX: " << x << "\tY: " << y << "\tPer: " << permission << "\t" << "\tB:" << buffer << "\tBusy: " << busy << std::endl;
  std::cout << "\t";
  displayNeighbours();
  if(gateway)
    std::cout << "\tGW";
  std::cout << std::endl;
}

/*
 * Returns the distance from this MR to the specified MR
 */
double MeshRouter::distance(MeshRouter MR)
{
  return sqrt( (x-MR.getX())*(x-MR.getX()) + (y-MR.getY())*(y-MR.getY()));
}

/*
 * Adds the neighbour_id to the neighbours vector only if unique, otherwise
 * stops execution
 */
void MeshRouter::addNeighbour(int neighbour_id)
{
  for(unsigned int x=0; x<neighbours.size();x++)
  {
    if(neighbours[x] == neighbour_id)
    {
      std::cout << "Error, this neighbour already exists for MR: " << id << std::endl;
      exit(255);
    }
  }
  neighbours.push_back(neighbour_id);
}

/* ---- Private Functions ---- */
void MeshRouter::displayNeighbours()
{
  std::cout << "Neighbours: ";
  for(unsigned int n=0; n<neighbours.size(); n++)
  {
    std::cout << neighbours[n];
    if(n+1 != neighbours.size())
      std::cout << ",";
  }
}

#endif
