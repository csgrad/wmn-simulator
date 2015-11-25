#ifndef SIMULATION_H
#define SIMULATION_H

/* Simulation Parameters */
#define REPEATS           10     //how many times sim should repeat for more consistent results

#define FS_ENABLED        1
#define GW_LOADBALANCING  1       //currently not used
#define NUM_GWS           1

#define START             0     //time in seconds
#define END               300
#define NUMPACKETS        10000
#define MEAN              0.1 //inter-arrival time
#define HOPDELAY          0.01
#define MAX_RETRIES       1
#define MAX_RETRY_TIME    0.05

#define UPDOWN_RATIO      1.0   //ratio of uplink packets to downlink packets

#define MAX_X             80
#define MAX_Y             80
#define MR_RANGE          120
#define MC_RANGE          60

#define DEBUGGING         0     //toggles debugging information
#define DISPLAY_PROGRESS  1     //toggles displaying % completed

#include <iostream>       //std::cout/cin, srand48, drand48
#include <queue>          //std::priority_queue
#include <vector>         //std::vector
#include <limits>         //numeric_limits

/* Packet Types */
#define EMPTY -1
#define NEW 0
#define HOP 1
#define DROP 2
#define SUCCESS 3
#define SCHEDULE 4
#define PERMISSION_START 5
#define PERMISSION_STOP 6

#include "Event.h"
#include "MeshRouter.h"
#include "Link.h"
#include "Matrix.h"

int main();

class Simulation
{
  public:
  /* Constructors */
  Simulation();
  
  /* Generation functions */
  void generateHexagonalTopology();
  void generateLinks();
  void generateNewPacket();
  
  void start();
  
  /* Display */
  void displayMRs();
  void displayLinks();
  void displayStats();
  
  /* Stats */
  double getAvgDelay(){return 
((double)low_delay/(double)packet_counter+(double)high_delay/(double)packet_counter)/2;};
  double getPacketDeliveryRatio(){return (double)((double)success_counter / (double)packet_counter) * 100.0;};
  int getNumMRs(){return (int)MR.size();};
  int getNumGWs(){return (int)GW.size();};
  
  private:
  /* Event Functions */
  void p_new(Event E);
  void p_hop(Event E);
  void p_success(Event E);
  void p_drop(Event E);
  void p_schedule(Event E);
  void p_start(Event E);
  void p_stop(Event E);
  
  /* Helper Functions */
  int nearestMeshRouter(double x, double y);
  int nearestNeighbour(unsigned int router_id, int destination);
  int nearestGW(int router_id);
  int findLink(int source, int destination);
  void pause();
  
  /* Generation Functions */
  void generateNeighbours();
  void generateGWs();
  
  std::priority_queue<Event> EQ;
  std::vector<MeshRouter> MR;
  std::vector<Link> LS;
  std::vector<int> GW;
  
  /* statistic variables */
  double now;
  int packet_counter, success_counter, drop_counter, progress;
  double high_delay;      //over-estimate using dropped delays
  double low_delay;       //under-estimate using only success delays
};

#include "Simulation.cc"

#endif
