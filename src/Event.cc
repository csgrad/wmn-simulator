/*
 * C++ Discrete Event Simulation
 * for Wireless Mesh Networks
 * by Jason Ernst
 * University of Guelph, ON, Canada
 * Revision 2 September 2008
 * 
 * Event Source Code
 */

#ifndef EVENT_CC
#define EVENT_CC

#include "Event.h"

/*
 * Determine which Event has a lower time
 * for the priority queue (note a > b causes lowest numbers to be on top of queue)
 */
bool operator<(const Event &a, const Event &b)
{
  return a.getTime() > b.getTime();
}

/*
 * Display the event
 */
void Event::display()
{
  std::cout << "  E: " << type << "\tS: " << source << "\tD: " << destination << "\t T: " << time << std::endl;
}

#endif
