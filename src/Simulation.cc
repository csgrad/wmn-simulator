/*
 * C++ Discrete Event Simulation
 * for Wireless Mesh Networks
 * by Jason Ernst
 * University of Guelph, ON, Canada
 * Revision 2 September 2008
 * 
 * Main Simulation Source Code
 */

#ifndef SIMULATION_CC
#define SIMULATION_CC

#include "Simulation.h"

/*
 * Main Function
 */
int main()
{
  double totalDelay = 0;
  double totalPacketDeliveryRatio=0;
  int numMRs = 0;
  int numGWs = 0;
  
  for(int repeat=0; repeat < REPEATS; repeat++)
  {
    Simulation sim;
  
    /* Initialize the Simulator */
    sim.generateHexagonalTopology();

    std::cout << "MR: " << sim.getNumMRs() << std::endl;
  
    if(FS_ENABLED)
    { sim.generateLinks(); }
  
    sim.start();
    
    /* Stats */
    totalDelay = totalDelay + sim.getAvgDelay();
    totalPacketDeliveryRatio = totalPacketDeliveryRatio + sim.getPacketDeliveryRatio();
    numMRs = sim.getNumMRs();
    numGWs = sim.getNumGWs();
    
    /* Display Statistics for each run*/
    sim.displayStats();
    
    /* Progress */
    if(DISPLAY_PROGRESS)
    {
      std::cout << "Progress: Run " << repeat+1 << " out of " << REPEATS 
<< std::endl;
    }
  }
  std::cout << "Successfully ran " << REPEATS << " runs. Stats: " << std::endl;
  std::cout << "AVG DELAY: " << totalDelay / (double)REPEATS << std::endl;
  std::cout << "AVG PDR: " << totalPacketDeliveryRatio / (double)REPEATS << std::endl;
  std::cout << "MRs: " << numMRs << " GWs: " << numGWs << std::endl;
  return 0;
}

Simulation::Simulation()
{
  srand48(time(NULL));
  now=0;
  packet_counter=0;
  success_counter=0;
  drop_counter=0;
  high_delay=0;
  low_delay=0;
  progress=0;
}

/*
 * Main Simulation Section of the program
 */
void Simulation::start()
{
  std::cout << "Starting Simulation..." << std::endl;
    
  generateNewPacket();
  
  /* Push a SCHEDULE event */
  if(FS_ENABLED)
  {
    Event nschedule(now, EMPTY, EMPTY, SCHEDULE);
    EQ.push(nschedule);
  }
  
  /* Main Simulation Loop */
  while(!EQ.empty() && packet_counter < NUMPACKETS && now < END)
  {
    /* Get the most recent event off the queue */
    Event E = EQ.top(); EQ.pop();
    now = E.getTime();
    
    if(DEBUGGING)
    {
      displayMRs();
      pause();
    }
    
    switch(E.getType())
    {
      case NEW:
        p_new(E);
      break;
      
      case HOP:
        p_hop(E);
      break;
      
      case SUCCESS:
        p_success(E);
      break;
      
      case DROP:
        p_drop(E);
      break;
      
      case SCHEDULE:
        p_schedule(E);
      break;
      
      case PERMISSION_START:
        p_start(E);
      break;
      
      case PERMISSION_STOP:
        p_stop(E);
      break;
      
      default:
        std::cout << "Unrecognized event type!" << std::endl;
        exit(255);
    }
    
    /* Generate new packets while the Simulation should still be running */
    if(packet_counter < NUMPACKETS && now < END)
      generateNewPacket();
    
    /* Display Progress of Simulation */
    if(DISPLAY_PROGRESS)
      if((int)(((double)packet_counter / (double)NUMPACKETS)*100.0) != progress)
      {
        progress = (int)(((double)packet_counter / (double)NUMPACKETS)*100.0);
        std::cout << "Progress: " << progress << "% \tTime: " << now << "\tPackets: " << packet_counter << std::endl;
      }
  }
  std::cout << "Simulation Complete." << std::endl;
}

/*
 * Function which generates the topology of the network based
 * on the Simulation parameters specified in Simulation.h
 */
void Simulation::generateHexagonalTopology()
{
  std::cout << "Generating the Topology...";
  
  //helpful variables
  double x=0, y=0;
  bool indentRow = true;
  int currentID=0;
  
  /* calculate the distances in x and y direction that MRs will be spread apart */
  double xDist = (double)3/4 * (double)MR_RANGE;
  double yDist = xDist;
  
  
  //add rows of MRs
  while(y < MAX_Y + yDist)
  {
    if(indentRow)
    { x=0; indentRow = false;}
    else
    { x=xDist/2; indentRow = true;}
    
    //add cols of MRs
    while(x < MAX_X + xDist)
    {
      MeshRouter temp(currentID, x, y);
      if(!FS_ENABLED)
        temp.setPermission(true);
      MR.push_back(temp);
      currentID++;
      x=x+xDist;
    }
    y=y+yDist;
  }
  
  generateNeighbours();
  generateGWs();
    
  std::cout << "complete." << std::endl;
}

void Simulation::generateLinks()
{
  std::cout << "Generating links...";
  
  int currentID = 0;
  
  for(unsigned int x=0;x<MR.size();x++)
  {
    std::vector<int> currentNeighbours = MR[x].getNeighbours();
    for(unsigned int n=0; n<currentNeighbours.size(); n++)
    {
      Link temp(currentID, MR[x].getID(), currentNeighbours[n]);
      currentID++;
      LS.push_back(temp);
    }
  }
  
  std::cout << "done." << std::endl;
}

/*
 * Function which is called each time a new packet is created
 * Supports both uplink and downlink packets.
 * 
 * For just uplink set UPDOWN_RATIO = 1
 * For just downlink set UPDOWN_RATIO = 0;
 */
void Simulation::generateNewPacket()
{
  /* 
   * Uplink or Downlink depends on ratio specified in parameters *
   * Uplink: from MCs to GWs
   * Downlink: from GWs to MCs
   */
  double chance = drand48();
  double x = drand48() * MAX_X;
  double y = drand48() * MAX_Y;
  
  int source=-1; int destination=-1;
  
  //uplink
  if(chance < UPDOWN_RATIO)
  {
    source = nearestMeshRouter(x,y);
    destination = MR[nearestGW(source)].getID();
  }
  //downlink
  else
  {
    destination = nearestMeshRouter(x,y);
    source = MR[nearestGW(destination)].getID();
  }
  
  double time = now + (-MEAN * log(drand48()));
  Event temp(time, source, destination, NEW);
  EQ.push(temp);
}

void Simulation::displayMRs()
{
  std::cout << "Displaying Mesh Routers: " << std::endl;
  for(unsigned int x=0;x<MR.size();x++)
    MR[x].display();
  std::cout << "Success: " << success_counter << "\tDropped: " << drop_counter << std::endl;
}

void Simulation::displayLinks()
{
  std::cout << "Displaying Links: " << std::endl;
  for(unsigned int x=0;x<LS.size();x++)
    LS[x].display();
}

void Simulation::displayStats()
{
  if(FS_ENABLED)
    std::cout << "FS ENABLED" << std::endl;
  else
    std::cout << "NO FS" << std::endl;
  std::cout << "Number of MRs: " << MR.size() << "\tNumber of Links: " << LS.size() << "\t# GWs: " << GW.size() << std::endl;
  std::cout << "Pkts: " << packet_counter << "\tSuccess: " << success_counter << "\tDropped: " << drop_counter << "\tS%: " << (double)((double)success_counter / (double)packet_counter) * 100.0 << std::endl;
  std::cout << "HOPDELAY: " << HOPDELAY << "\t\tMEAN: " << MEAN << std::endl;
  std::cout << "Ttl HDelay: " << high_delay << "\tTtl LDelay: " << low_delay << std::endl;
  std::cout << "Avg HDelay: " << (double)high_delay / (double)packet_counter << "Avg LDelay: " << (double)low_delay / (double)packet_counter << std::endl;
}

/* ---- Private Functions ---- */

/*
 * Generates all possible neighbours for every MR in the topology
 * Two MRs are neighbours if they are within MR_RANGE specified in the
 * Simulation parameters set by Simulation.h
 */
void Simulation::generateNeighbours()
{
  for(unsigned int currentMR=0; currentMR < MR.size(); currentMR++)
  {
    for(unsigned int potentialNeighbour=0; potentialNeighbour < MR.size(); potentialNeighbour++)
    {
      if(currentMR != potentialNeighbour)
      {
        if(MR[currentMR].distance(MR[potentialNeighbour]) < double(MR_RANGE))
          MR[currentMR].addNeighbour(MR[potentialNeighbour].getID());
      }
    }
  }
}

/*
 * Randomly selects MRs for GWs based on the number of GWs
 * specified in the parameters for the Simulation in Simulation.h
 */
void Simulation::generateGWs()
{
  if(MR.size() < NUM_GWS)
  {
    std::cout << "Error you have specified more GWs than there are MRs in the simulation. You may fix this by changing the MAX_X and MAX_Y or the NUM_GWS" << std::endl;
    exit(255);
  }
  
  for(int x=0;x<NUM_GWS;x++)
  {
    int gw = (int)(drand48()*MR.size());
    while(MR[gw].isGateway())
      gw = (int)(drand48()*MR.size());
    MR[gw].setGateway();
    GW.push_back(MR[gw].getID());
  }
}

/*
 * Returns the id of the MR which is the closest to a given point
 */
int Simulation::nearestMeshRouter(double x, double y)
{
  double minDist=std::numeric_limits<double>::max();
  int minID=std::numeric_limits<int>::max();
  
  for(unsigned int c=0;c<MR.size();c++)
  {
    double distance = sqrt((MR[c].getX()-x)*(MR[c].getX()-x)+(MR[c].getY()-y)*(MR[c].getY()-y));
    if(distance <= double(MC_RANGE) && distance < minDist)
    { minDist = distance; minID = c; }
  }
  
  if(minID == std::numeric_limits<int>::max())
  {
    std::cout << "Error, could not associate the new packet with a MR." <<
      " You may need to adjust your ranges because there are dead spots in the network coverage." << std::endl;
    exit(255);
  }
  
  return minID;
}

/*
 * Returns the nearest GW to a given MR
 */
int Simulation::nearestGW(int router_id)
{
  double minDist=std::numeric_limits<double>::max();
  int minID=std::numeric_limits<int>::max();
  
  for(unsigned int c=0;c<GW.size();c++)
  {
    int gw_index = GW[c];
    double distance = sqrt((MR[gw_index].getX()-MR[router_id].getX())*(MR[gw_index].getX()-MR[router_id].getX())+(MR[gw_index].getY()-MR[router_id].getY())*(MR[gw_index].getY()-MR[router_id].getY()));
    if(distance < minDist)
    { minDist = distance; minID = gw_index; }
  }
  
  if(minID == std::numeric_limits<int>::max())
  {
    std::cout << "Error, could not associate the new packet with a MR." <<
      " You may need to adjust your ranges because there are dead spots in the network coverage." << std::endl;
    exit(255);
  }
  
  return minID;
}

/*
 * Finds the nearest neighbour between router_id and destination
 */
int Simulation::nearestNeighbour(unsigned int router_id, int destination)
{
  double minDist=std::numeric_limits<double>::max();
  int minID=std::numeric_limits<int>::max();
  std::vector<int> neighbours = MR[router_id].getNeighbours();
  
  for(unsigned int n=0; n<neighbours.size(); n++)
  {
    int neighbour = neighbours[n];
    double distance = sqrt((MR[neighbour].getX()-MR[destination].getX())*(MR[neighbour].getX()-MR[destination].getX())+(MR[neighbour].getY()-MR[destination].getY())*(MR[neighbour].getY()-MR[destination].getY()));
    if(distance < minDist)
    { minDist = distance; minID = neighbour; }
  }
  
  if(minID == std::numeric_limits<int>::max())
  {
    std::cout << "Error, could not find the nearest neighbour to MR:" << router_id << std::endl;
    exit(255);
  }
  
  return minID;
}

/*
 * Returns the id of the link with a given source and destination. If not
 * found display and error
 */
int Simulation::findLink(int source, int destination)
{
  for(unsigned int l=0;l<LS.size();l++)
  {
    if(LS[l].getSource()==source && LS[l].getDestination() == destination)
      return l;
  }
  std::cout << "Could not find the link with S: " << source << " and D: " << destination << std::endl;
  exit(255);
  return -1;
}

/*
 * Handle a packet from MC to MR
 */
void Simulation::p_new(Event E)
{
  int source = E.getSource();
  int destination = E.getDestination();
  
  if(DEBUGGING)
    std::cout << "  NEW S: " << source << " D: " << destination << " Time: " << now << std::endl;
    
  if(!MR[source].isBusy() && MR[source].getBuffer() == EMPTY)
  {
    packet_counter++;
    MR[source].setBuffer(destination);
  
    /* 
     * try to guess at which links will be used
     * based on the closest neighbour and add a requirement
     * all the way between them and the destination
     */
    if(FS_ENABLED)
    {
      int tempsource = source;
    
      while(tempsource!=destination)
      {
        int neighbour = nearestNeighbour(tempsource, destination);
        int l = findLink(tempsource, neighbour);
        LS[l].setRequirement(LS[l].getRequirement()+1);
        tempsource = neighbour;
      }
    }
  
    /* 
     * Make a new HOP event, incremement the time and reinsert into priority queue
     */
    Event hop(now+HOPDELAY,source,destination,HOP);
    hop.setDelay(HOPDELAY);
    MR[source].setBuffer(destination);
    MR[source].setBusy(true);
    EQ.push(hop);
  }
  //MR buffer full or busy
  else
  {
    /*
    Event drop(now, source, destination, DROP);
    EQ.push(drop);
    */
  }
}

void Simulation::p_hop(Event E)
{
  int source = E.getSource();
  int destination = E.getDestination();
  double delay = E.getDelay();
  
  /* Already Arrived */
  if(source == destination)
  {
    Event success(now, source, destination, SUCCESS);
    success.setDelay(delay);
    EQ.push(success);
    if(DEBUGGING)
      std::cout << "  HOP S: " << source << " D: " << destination << " Time: " << now << std::endl;
  }
  /* Attempt to HOP */
  else
  {
    int neighbour = nearestNeighbour(source, destination);
    if(DEBUGGING)
      std::cout << "  HOP S: " << source << " to N: " << neighbour << " with D: " << destination << " Time: " << now << std::endl;
      
    /* Free to HOP */
    if(!MR[neighbour].isBusy() && MR[neighbour].getBuffer() == EMPTY && MR[source].hasPermission() && MR[neighbour].hasPermission())
    {
      MR[neighbour].setBusy(true);
      MR[source].setBusy(false);
      
      MR[neighbour].setBuffer(destination);
      MR[source].setBuffer(EMPTY);
      
      /* Clear the link requirement of source->neighbour once we have hopped */
      if(FS_ENABLED)
      {
        int l=findLink(source,neighbour);
        LS[l].setRequirement(LS[l].getRequirement()-1);
      }
      
      Event hop(now+HOPDELAY, neighbour, destination, HOP);
      hop.setDelay(delay+HOPDELAY);
      EQ.push(hop);
    }
    /* Neighbour Busy or Full or Either Nodes has no permission */
    else
    {
      /* Retry if we haven't exceeded the max # of retries */
      if(!FS_ENABLED || MR[source].getRetries() > MAX_RETRIES)
      {
        Event drop(now, source, destination, DROP);
        drop.setDelay(delay);
        EQ.push(drop);
      }
      else
      {
        //std::cout << "  RETRY" << std::endl;
        double d = drand48() * MAX_RETRY_TIME;
        Event hop(now+d, source, destination, HOP);
        hop.setDelay(delay + d);
        MR[source].setRetries(MR[source].getRetries() + 1);
        EQ.push(hop);
      }
    }
  }
}

void Simulation::p_success(Event E)
{    
  int source=E.getSource();
  double delay=E.getDelay();
  
  if(DEBUGGING)
    std::cout << "  SUCCESS: " << source << " Time: " << now << std::endl;
  
  MR[source].setBusy(false);
  MR[source].setBuffer(EMPTY);
  MR[source].setRetries(0);
  
  success_counter++;
  high_delay = high_delay + delay;
  low_delay = low_delay + delay;
}

void Simulation::p_drop(Event E)
{
  int source = E.getSource();
  int destination = E.getDestination();
  double delay = E.getDelay();
  
  if(DEBUGGING)
    std::cout << "  DROP S: " << source << " D: " << destination << " Time: " << now << std::endl;
  
  /* 
   * try to guess at which links will be used
   * based on the closest neighbour and remove a requirement
   * all the way between them and the destination
   */
  if(FS_ENABLED)
  {
    int tempsource = source;
  
    while(tempsource!=destination)
    {
      int neighbour = nearestNeighbour(tempsource, destination);
      int l = findLink(tempsource, neighbour);
      LS[l].setRequirement(LS[l].getRequirement()-1);
      tempsource = neighbour;
    }
  }
  
  MR[source].setBusy(false);
  MR[source].setBuffer(EMPTY);
  MR[source].setRetries(0);
  
  high_delay = high_delay + delay;
  drop_counter++;
}

/*
 * Generates a new schedule and all of the
 * start and stop permission events that go with it
 */
void Simulation::p_schedule(Event E)
{    
  Matrix CM(LS);
  std::vector <Clique> scheduling = CM.generateScheduling();
  
  if(DEBUGGING)
  {
    std::cout << "  GEN SCHEDULE: Time: " << now << std::endl;
  
    std::cout << "active links: " << std::endl;
    for(unsigned int x=0;x<LS.size();x++)
      if(LS[x].getRequirement() > 0)
        LS[x].display();
    std::cout << std::endl;
  
    std::cout << "scheduling: " << std::endl;
    for(unsigned int x=0;x<scheduling.size();x++)
      scheduling[x].display();
    std::cout << std::endl;
  }
  
  double start = now;
  double stop = now;

  for(unsigned int c=0;c<scheduling.size();c++)
  {
    Clique schedule = scheduling[c];
    int weight = schedule.getWeight(LS);
    stop = stop + (weight * HOPDELAY);
    
    for(unsigned int l=0;l<schedule.size();l++)
    {
      int link_id = schedule.getLink(l);
      if(DEBUGGING)
        std::cout << "  L: " << link_id << " T: " << start << " - " << stop << ": " << LS[link_id].getSource() << " & " << LS[link_id].getDestination() << std::endl;
      Event pstart(start,LS[link_id].getSource(), LS[link_id].getDestination(), PERMISSION_START);
      Event pstop(stop, LS[link_id].getSource(), LS[link_id].getDestination(), PERMISSION_STOP);
      EQ.push(pstart);
      EQ.push(pstop);
    }
    start = stop;
  }
  Event nschedule(start+HOPDELAY, EMPTY, EMPTY, SCHEDULE);
  EQ.push(nschedule);
}

void Simulation::p_start(Event E)
{
  int source = E.getSource();
  int destination = E.getDestination();
  MR[source].setPermission(true);
  MR[destination].setPermission(true);
  
  if(DEBUGGING)
    std::cout << "  GIVE PERMISSION: " << source << " & " << destination << " Time: " << now << std::endl;
}

void Simulation::p_stop(Event E)
{
  int source = E.getSource();
  int destination = E.getDestination();
  MR[source].setPermission(false);
  MR[destination].setPermission(false);
  
  if(DEBUGGING)
    std::cout << "  STOP PERMISSION: " << source << " & " << destination << " Time: " << now << std::endl;
}

void Simulation::pause()
{
  std::cout << "Press any key to continue...";
  std::cin.ignore(0,'\n');
  std::getchar();
}

#endif
