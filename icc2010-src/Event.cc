#ifndef EVENT_CC
#define EVENT_CC

#include "Event.h"

/*
 * Construct and event giving no parameters
 */
Event::Event()
{
	type = -1;
	source = -1;
	destination = -1;
	time = -1;
	start_time = -1;
}

/*
 * Constructor for an event which has all of the required parameters
 */
Event::Event(int _type, int _source, int _destination, double _time, double _start_time)
{
	type = _type;
	source = _source;
	destination = _destination;
	time = _time;
	start_time = _start_time;
}

/*
 * Determine which Event has a lower time
 * for the priority queue (note a > b causes lowest numbers to be on top of queue)
 */
bool operator<(Event a, Event b)
{
  return a.getTime() > b.getTime();
}

int Event::getType()
{ return type;}

int Event::getSource()
{ return source;}

int Event::getDestination()
{ return destination;}

double Event::getTime()
{ return time;}

double Event::getStartTime()
{ return start_time;}

#endif
