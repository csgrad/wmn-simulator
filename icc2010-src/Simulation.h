#ifndef SIMULATION_H
#define SIMULATION_H

#include "MeshRouter.h"
#include "MeshClient.h"
#include "Link.h"
#include "TopologyHelper.h"
#include "Event.h"
#include "Clique.h"

/* Simulation Parameters */
#define REPEATS						10		//how many times we should repeat for consistent results
#define START							0.0		//start time (s)
#define END								300.0	//end time (s)
#define NUMPACKETS				10000	//number of packets
#define MEAN							0.01		//mean inter-arrival time
#define HOPDELAY					0.01		//the maximum average delay for each hop
#define MAX_X							1000	//maximum x dimension of the simulation environment in meters
#define MAX_Y							1000	//maximum y dimension of the simulation environment in meters
#define NUM_MC						250   //the number of MCs in the simulation
#define RANGE							250		//maximum communication range of MRs and MCs in meters

/* Mixed-Bias Parameters */
#define A1								0.2		//alpha 1 -> proportion to use b11 bias (1-a1) uses b12
#define B11								5			//strong bias exponent against greedy flows
#define B12								1     //weak bias exponent against greedy flows (proportional fair component when b12=1)

#define A2								0.2		//alpha 2 -> proportion to use b21 bias (1-a2) uses b22
#define B21								5			//strong bias exponent against many hops
#define B22								1			//weak bias exponent against many hops

#define G									0.5		//gamma -> what proportion of resources to assign to each strategy

/* Simulation Options */
#define DEBUGGING					1			//toggle debug on/off
#define DISPLAY_PROGRESS	1			//toggle display % complete on/off
 
#include <iostream>
#include <queue>

class Simulation
{
	public:
		Simulation(int _num_mr, int _num_gw, int _num_mc, bool _fs_enabled, bool _mb_enabled);
		void pause();
	private:
		void displayMRs();
		void displayMCs();
		void displayLSs();
		void initializeTopology();
		void start();
		void run();
		
		//simulation event functions
		Event generatePacket();
		int getNeighbour(int router);
		double getDelay();
		
		//schedule generation functions
		void addRequirement(int router);
		void removeRequirement(int router);
		std::vector <Clique> generateSchedule();
		std::vector <Clique> generateAllCliques();
		bool unique(std::vector <Clique> cliques, Clique c);
		bool interfere(Link l1, Link l2);
		double distance(int source, int destination);
		bool compatibleLink(Link l, Clique c);
		
		
		//important topology information
		std::vector <MeshRouter> MR;
		std::vector <MeshClient> MC;
		std::vector <MeshRouter> GW;
		std::vector <Link> LS;
		TopologyHelper topology;
		
		//event queue
		std::priority_queue<Event> EQ;
		
		//important simulation variables
		int num_gw, num_mr, num_mc, current_run;
		double now;
		bool fs_enabled, mb_enabled;
		
		//statistics variables
		double total_delay;
		double total_pdr;
};

#endif
