#ifndef EVENT_H
#define EVENT_H

#define MCPACKET -2
#define NEW 0
#define HOP 1
#define DROP 2
#define SUCCESS 3
#define SCHEDULE 4
#define P_START 5
#define P_STOP 6

class Event
{
	public:
		Event();
		Event(int _type, int _source, int _destination, double _time, double _start_time);
		
		int getType();
		int getSource();
		int getDestination();
		double getTime();
		double getStartTime();
	private:
		int type, source, destination;
		double time, start_time;
};

bool operator<(Event a, Event b);

#endif
