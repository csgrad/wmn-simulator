#ifndef EVENT_H
#define EVENT_H

#include <iostream>       //std::cout/cin, srand48, drand48

class Event
{
  public:
  /* Constructors */
  Event(){time=-1;type=-1;source=-1;destination=-1; cumulative_delay=0;};
  Event(double t, int s, int d, int e_type){time = t; source=s; destination=d;type=e_type; cumulative_delay=0;};
  
  /*
   * Determine which Event has a lower time
   * for the priority queue (note a > b causes lowest numbers to be on top of queue)
   */
   friend bool operator<(const Event &a, const Event &b)
   {
	  return a.getTime() > b.getTime();
   }
  
  /* Accessors */
  double getTime() const {return time;}; 
  double getDelay() {return cumulative_delay;};
  int getSource(){return source;};
  int getDestination(){return destination;};
  int getType(){return type;};
  
  /* Mutators */
  void setType(int e_type){type=e_type;};
  void setDelay(double d){cumulative_delay=d;};
  
  /* Display */
  void display();
  
  private:
  double time; double cumulative_delay;
  int source,destination, type;
};

#endif
