/*
 * C++ Discrete Event Simulation
 * for Wireless Mesh Networks
 * by Jason Ernst
 * University of Guelph, Guelph ON, Canada
 * Version 0.3: December 2008
 * 
 * Main Simulation Source Code
 */

#ifndef SIMULATION_CC
#define SIMULATION_CC

#include "Simulation.h"

int main()
{
	srand48(time(NULL)); //randomize the generator
	std::cout << "C++ Discrete Event Simulation" << std::endl;
	std::cout << "Jason Ernst, University of Guelph" << std::endl;
	std::cout << "Version 0.4, January 2009" << std::endl;
	std::cout << "---------------------------------" << std::endl;

	int num_mr, num_gw;
	
	
	/* Simulation Runs without FS */	
	num_mr = 10;
	
	//loop between 10 and 40 mesh routers, incrementing by 5 each time
	while(num_mr <= 50)
	{
		num_gw = 1;
		
		//run each mesh router result with gws between 1 and 5
		while(num_gw <= 5)
		{
			Simulation sim = Simulation(num_mr, num_gw, NUM_MC, false, false);
			num_gw++;
		}
		num_mr += 5;
	}
		
	/* Simulation Runs with FS */
	num_mr = 10;
	
	//loop between 10 and 40 mesh routers, incrementing by 5 each time
	while(num_mr <= 50)
	{
		num_gw = 1;
		
		//run each mesh router result with gws between 1 and 5
		while(num_gw <= 5)
		{
			Simulation sim = Simulation(num_mr, num_gw, NUM_MC, true, false);
			num_gw++;
		}
		num_mr += 5;
	}
	
	/* Simulation Runs with Mixed-Bias */
	num_mr = 10;
	
	while(num_gw <= 50)
	{
		num_gw = 1;
		
		//run each mesh router result with gws between 1 and 5
		while(num_gw <= 5)
		{
			Simulation sim = Simulation(num_mr, num_gw, NUM_MC, true, true);
			num_gw++;
		}
		num_mr += 5;
	}
	
	return 0;
}

/*
 * Create a new instance of a Simulation with certain parameters set for
 * the duration of this simulation
 */
Simulation::Simulation(int _num_mr, int _num_gw, int _num_mc, bool _fs_enabled, bool _mb_enabled)
{
	//initialize the important simulation variables
	current_run = 1;
	num_mr = _num_mr;
	num_gw = _num_gw;
	num_mc = _num_mc;
	fs_enabled = _fs_enabled;
	mb_enabled = _mb_enabled;
	srand48(time(NULL));
	
	//initialize the statistics variables
	total_delay = 0;
	total_pdr = 0;
	
	std::cout << "New Simulation Instance: MR: " << num_mr << " GW: " << num_gw << " FS_ENABLED: " << fs_enabled << " MB_ENABLED: " << mb_enabled << std::endl;
	
	start();
}

/*
 * Starts the simulation
 */
void Simulation::start()
{
	if(REPEATS < 1)
	{
		std::cout << "Error, REPEATS must be at least 1" << std::endl;
		exit(255);
	}
	
	/* Important! Must generate the topology and perform expensive
	 * calculations here as much as possible to lower computing time
	 */
	topology = TopologyHelper(MAX_X, MAX_Y, num_mr, num_gw, num_mc, RANGE);
	MR = topology.getMR();
	displayMRs();
	GW = topology.getGW();
	MC = topology.getMC();
	LS = topology.getLS();
	//displayLSs();
	//displayMCs();
	
	while(current_run <= REPEATS)
	{		
		run();
		current_run++;
	}
	//std::cout << "Final Results, " << REPEATS << " repeats." << std::endl;
	std::cout << "Average Delay: " << total_delay / REPEATS << " Average PDR: " << total_pdr / REPEATS << std::endl;
	//pause();
}

/*
 * Executes a Simulation Run
 */
void Simulation::run()
{
	//std::cout << "  Executing Run: " << current_run << std::endl;
	
	//initialize statistics for this run
	now = 0;
	initializeTopology();
	double delay = 0;
	int packet_counter = 0;
	int success_packets = 0;
	int dropped_packets = 0;
	
	Event E;
	E = generatePacket();
	EQ.push(E);
	std::vector<Clique> scheduling;
	
	while(!EQ.empty() && now < END && packet_counter < NUMPACKETS)
	{
		E = EQ.top(); EQ.pop();
		
		//get information from event
		double time = E.getTime();
		double start_time = E.getStartTime();
		int source = E.getSource();
		int destination = E.getDestination();
		int type = E.getType();
		int neighbour = -1;
		now = time;
		
		//variables for updating requirement tables
		int current_router;
		int current_gw;
		
		//variables for performing scheduling
		int x,y, weight;
		double start_schedule, stop_schedule;
		
		switch(type)
		{
			case NEW:
				//std::cout << "    New Event, t=" << now << " MC: " << source << " (" << MC[source].getX() << "," << MC[source].getY() << ")" << " MR: " << destination << " (" << MR[destination].getX() << "," << MR[destination].getY() << ")" << std::endl;
				//MR[destination].display();
				//pause();
				
				//ensure that the MR buffer is empty
				if(MR[destination].getBuffer() == -1)
				{
					packet_counter++;
					MR[destination].setBuffer(MCPACKET);
					
					//check if we already have success
					if(MR[destination].isGW())
						E = Event(SUCCESS, destination, MCPACKET, now, start_time);
					else
					{
						neighbour = getNeighbour(destination);
						E = Event(HOP, destination, neighbour, now + getDelay(), start_time);
						
						//update requirement table
						current_router = destination;
						current_gw = MR[destination].getNearestGW();
						while(current_router != current_gw)
						{
							addRequirement(current_router);
							current_router = getNeighbour(current_router);
						}
					}
					
					EQ.push(E);
					
					//if this is the first packet, we must initiate a scheduling
					if(packet_counter == 1 && fs_enabled)
					{
						E = Event(SCHEDULE, -1, -1, now, now);
						EQ.push(E);
					}
				}
				else
				{
					//E = Event(DROP, MCPACKET, -1, now, start_time);
					//EQ.push(E);	
				} //drop for now if collision between MC and MR
			break;
			case HOP:
				//std::cout << "    Hop Event, t=" << now << " From MR: " << source << " to MR: " << destination << std::endl;
				//MR[source].display();
				//MR[destination].display();
				//pause();
				
				//ensure buffer is empty
				if(MR[destination].getBuffer() == -1)
				{
					//ensure that we have permission to send (always do with fs_enabled = 0)
					if(MR[destination].hasPermission())
					{
						MR[destination].setBuffer(source);
						MR[source].setBuffer(-1);
					
						//update requirement table
						if(source!=MCPACKET)
							removeRequirement(source);
											
						//check to see if we have arrived
						if(MR[destination].isGW())
							E = Event(SUCCESS, destination, -1, now + getDelay(), start_time);
						//hop if we havent'
						else
						{
							neighbour = getNeighbour(destination);
							E = Event(HOP, destination, neighbour, now + getDelay(), start_time);
						}
					}
					else
					{
						/***** NEED TO CHANGE THIS TO ONLY RETRY IF NOW - STARTIME < RETRY THRESHOLD ************/
						//std::cout << "No Permission to send packet" << std::endl;
						if(now - start_time < HOPDELAY)
							E = Event(HOP, source, destination, now + getDelay(), start_time); //retry
						else
							E = Event(DROP, source, -1, now, start_time);
					}
				}
				//drop packet if buffer not available
				else
				{
					E = Event(DROP, source, -1, now, start_time);
				}
				
				EQ.push(E);
			break;
			case DROP:
				//std::cout << "    Drop Event, t=" << now << " at MR: " << source << std::endl;
				//pause();
				
				dropped_packets++;
				delay += now - start_time;
				if(source != MCPACKET)
				{
					MR[source].setBuffer(-1);
				
					//update requirement table
					current_router = source;
					current_gw = MR[source].getNearestGW();
					while(current_router != current_gw)
					{
						removeRequirement(current_router);
						current_router = getNeighbour(current_router);
					}
				}
			break;
			case SUCCESS:
				//std::cout << "    Success Event, t=" << now << " at MR: " << source << std::endl;
				//pause();
				
				success_packets++;
				delay += now - start_time;
				if(source != MCPACKET)
					MR[source].setBuffer(-1);
			break;
			case SCHEDULE:
				//std::cout << "    Schedule Event, t=" << now << std::endl;
				//pause();
				
				//std::cout << "packets: " << packet_counter << " now: " << now << std::endl;
				
				scheduling = generateSchedule();
				
				start_schedule = now;
				
				//go through each round in the scheduling
				for(x=0;x<(int)scheduling.size();x++)
				{
					//scheduling[x].display();
					weight = scheduling[x].getWeight();
					stop_schedule = start_schedule + weight * getDelay();
					
					//create the start and stop events for each link in the current schedule round
					for(y=0;y<(int)scheduling[x].links.size();y++)
					{
						source=scheduling[x].links[y].first;
						E = Event(P_START, source, -1, start_schedule, now);
						EQ.push(E);
						E = Event(P_STOP, source, -1, stop_schedule, now);
						EQ.push(E);
						
						destination=scheduling[x].links[y].second;
						E = Event(P_START, destination, -1, start_schedule, now);	
						EQ.push(E);
						E = Event(P_STOP, destination, -1, stop_schedule, now);	
						EQ.push(E);
					}
					
					start_schedule = stop_schedule;
				}
				E = Event(SCHEDULE, -1, -1, start_schedule + getDelay(), now);
				EQ.push(E);
				//displayMRs();
				//pause();
			break;
			case P_START:
				//std::cout << "    Start Permission Event, t=" << now << " at MR: " << source << std::endl;
				//pause();
				MR[source].setPermission(true);
			break;
			case P_STOP:
				//std::cout << "    Stop Permission Event, t=" << now << " at MR: " << source << std::endl;
				//pause();
				MR[source].setPermission(false);
			break;
			default:
				std::cout << "Error, unrecognized Event. Simulation ending. " << std::endl;
				exit(255);
		}
		E = generatePacket();
		EQ.push(E);
	}
	
	//display statistics for this run
	double average_delay = 0;
	double pdr = 0;
	if(success_packets > 0)
		average_delay = delay / (double)success_packets;
	if(packet_counter > 0)
		pdr = success_packets / (double)packet_counter;
	
	total_delay+=average_delay;
	total_pdr+=pdr;

	//std::cout << "    Packets Generated: " << packet_counter << " Success: " << success_packets << " Dropped: " << dropped_packets << std::endl;
	//std::cout << "    Average Delay: " << average_delay << " PDR: " << pdr << std::endl;
}

/*
 * Causes the Simulation to Pause for user input
 */
void Simulation::pause()
{
	//std::cout << '\a';
	std::cin.get();
}

/*
 * Displays all of the Mesh Routers in the Simulation
 */
void Simulation::displayMRs()
{
	std::cout << "  Displaying Mesh Routers: " << std::endl;
	for(int x=0; x<(int)MR.size();x++)
		MR[x].display();
}

/*
 * Displays all of the Mesh Clients in the Simulation
 */
void Simulation::displayMCs()
{
	std::cout << "  Displaying Mesh Clients: " << std::endl;
	for(int x=0; x<(int)MC.size();x++)
		MC[x].display();
}

void Simulation::displayLSs()
{
	std::cout << "  Displaying Links: " << std::endl;
	for(int x=0; x<(int)LS.size();x++)
		LS[x].display();
}

/*
 * Ensures all buffers are empty, requirements cleared etc
 */
void Simulation::initializeTopology()
{
	for(int c=0; c<(int)MR.size();c++)
	{
		MR[c].setBuffer(-1);
		if(fs_enabled)
			MR[c].setPermission(false);
		else
			MR[c].setPermission(true);
	}
	
	for(int c=0; c<(int)LS.size();c++)
		LS[c].requirement = 0;
}

/*
 * Generates a new packet with mean inter-arrival time MEAN
 */
Event Simulation::generatePacket()
{
	double time;
	time = now + (-MEAN * log(drand48()));
	int source = (int)(drand48() * MC.size());
	int destination = MC[source].getMR();
	
	return Event(NEW, source, destination, time, time);
}

/*
 * Returns the next hop on the way to the gateway for a given router
 */
int Simulation::getNeighbour(int router)
{
	for(int x=0;x<(int)LS.size();x++)
	{
		if(LS[x].first == router)
			return LS[x].second;
	}
	
	std::cout << "Error, could not find the next-hop neighbour for router: " << router << ". Serious error." << std::endl;
	exit(255);
}

/*
 * Adds a requirement to the link with the given router as the "source"
 */
void Simulation::addRequirement(int router)
{
	for(int x=0;x<(int)LS.size();x++)
	{
		if(LS[x].first == router)
		{
			LS[x].requirement++;
			return;
		}	
	}
	std::cout << "Error, could not find the router: " << router << " for incrementing requiremnt. Serious error." << std::endl;
	exit(255);
}

/*
 * Removes a requirement to the link with the given router as the "source"
 */
void Simulation::removeRequirement(int router)
{
	for(int x=0;x<(int)LS.size();x++)
	{
		if(LS[x].first == router)
		{
			LS[x].requirement--;
			return;
		}	
	}
	std::cout << "Error, could not find the router: " << router << " for removing requiremnt. Serious error." << std::endl;
	exit(255);
}

/*
 * Generates a link scheduling based on the requirements at each link in the network
 */
std::vector <Clique> Simulation::generateSchedule()
{
	std::vector <Clique> schedule;
	std::vector <Clique> allCliques = generateAllCliques();
	//std::cout << "Generating scheduling..." << std::endl;

	//continue generating the schedule while there are still cliques left
	while(allCliques.size() > 0)
	{
		int max_gain=-1; int max_index=-1; int gain=0;
		for(int x=0;x<(int)allCliques.size();x++)
		{
			gain = allCliques[x].getGain();
			if(gain > max_gain)
			{
				max_gain = gain;
				max_index = x;
			}
		}
		
		//select the clique with the maximal gain and add it to the scheduling
		if(max_gain != -1 && allCliques[max_index].getWeight() > 0)
			schedule.push_back(allCliques[max_index]);
		
		Clique maxClique = allCliques[max_index];
		
		//remove all cliques which intersect with the maxClique
		for(int x=0; x<(int)allCliques.size();x++)
		{
			if(maxClique.intersect(allCliques[x]))
			{
				allCliques.erase(allCliques.begin() + x);
				x--; //step back 1 since we erased and element
			}
		}
	}	
	//std::cout << "done." << std::endl;
	return schedule;
}

/*
 * Generates all combinations of links which are compatible with each other
 * (links which can transmit together without interference)
 */
std::vector <Clique> Simulation::generateAllCliques()
{
	//std::cout << "Generating All Cliques..." << std::endl;
	std::vector <Clique> allCliques;
	for(int x=0; x<(int)LS.size();x++)
	{
		Clique c;
		
		if(LS[x].requirement > 0)
		{
			c.links.push_back(LS[x]);
						
			for(int y=0; y<(int)LS.size();y++)
			{
				if(LS[y].requirement > 0 && y!=x && compatibleLink(LS[y], c))
				{
					c.links.push_back(LS[y]);
				}
			}
		
			if(unique(allCliques,c))
			{
				//std::cout << "Adding cliques starting with link: " << x << " out of total: " << LS.size() << std::endl;
				//c.display();
				allCliques.push_back(c);
				std::vector <Clique> smallerCliques = c.decompose();
				for(int y=0;y<(int)smallerCliques.size();y++)
				{
					if(unique(allCliques, smallerCliques[y]))
						allCliques.push_back(smallerCliques[y]);
				}
			}
		}
	}
	
	/* push on single linked cliques so none which have a requirement are missed
	 */
	for(int x=0;x<(int)LS.size();x++)
	{
		if(LS[x].requirement>0)
		{
			//push on the single linked cliques
			Clique small;
			small.links.push_back(LS[x]);
			if(unique(allCliques, small))
				allCliques.push_back(small);
		}
	}
	
	//std::cout << "done." << std::endl;
	
	//for(int c=0;c<(int)allCliques.size();c++)
	//	allCliques[c].display();
	//pause();
	return allCliques;
}

/*
 * Returns true if the given clique is unique when compared to the vector of cliques
 */
bool Simulation::unique(std::vector <Clique> cliques, Clique c)
{
	//prevent empty cliques
	if(c.links.size() == 0)
		return false;
		
	for(int x=0; x<(int)cliques.size(); x++)
	{
		if(cliques[x].equivalent(c))
			return false;
	}
	return true;
}

/*
 * Returns true if the given link is compatible with the existing links within a Clique
 */
bool Simulation::compatibleLink(Link l, Clique c)
{
	for(int x=0;x<(int)c.links.size();x++)
	{
		if(interfere(l, c.links[x]))
			return false;
	}
	return true;
}

/*
 * Returns true if the two links interfere with one another
 */
bool Simulation::interfere(Link l1, Link l2)
{
	if(l1.first == l2.first || l1.first == l2.second || l2.first == l1.second || l2.second == l1.second)
		return true;
	else
	{
		int s1 = l1.first;
		int s2 = l2.first;
		int d1 = l1.second;
		int d2 = l2.second;
		
		if(distance(s1,s2) < RANGE || distance(s1, d2) < RANGE || distance(s2, d1) < RANGE)
			return true;	
		else
			return false;	
	}
}

/*
 * Returns the distance between two 2d points p1, p2
 */
double Simulation::distance(int source, int destination)
{
	double p1x = MR[source].getX();
	double p2x = MR[destination].getX();
	double p1y = MR[source].getY();
	double p2y = MR[destination].getY();
	
	return sqrt((p2x-p1x)*(p2x-p1x) + (p2y-p1y)*(p2y-p1y));
}

/*
 * Returns a random delay between 0 and HOPDELAY
 */
double Simulation::getDelay()
{
	return (-HOPDELAY * log(drand48()));
}
#endif
