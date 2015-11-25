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
 * Display the event
 */
void Event::display()
{
  std::cout << "  E: " << type << "\tS: " << source << "\tD: " << destination << "\t T: " << time << std::endl;
}

#endif
