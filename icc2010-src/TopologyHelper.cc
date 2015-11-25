#ifndef TOPOLOGYHELPER_CC
#define TOPOLOGYHELPER_CC

#include "TopologyHelper.h"

/*
 * Construct a new TopologyHelper
 */
TopologyHelper::TopologyHelper(int _max_x, int _max_y, int _num_mr, int _num_gw, int _num_mc, double _range)
{
	max_x = _max_x;
	max_y = _max_y;
	num_mr = _num_mr;
	num_gw = _num_gw;
	num_mc = _num_mc;
	range = _range;
	
	generateTopology();
	generateMeshClients();
	positionGWs();
  generateLinks();
}

/*
 * Default constructor, sets all values to -1
 */
TopologyHelper::TopologyHelper()
{
	max_x = -1;
	max_y = -1;
	num_mr = -1;
	num_gw = -1;
}

/*
 * Accessor Functions to return the MRs, GWs and MCs
 */
std::vector<MeshRouter> TopologyHelper::getMR()
{	return MR; }
std::vector<MeshRouter> TopologyHelper::getGW()
{	return GW; }
std::vector<MeshClient> TopologyHelper::getMC()
{	return MC; }
std::vector<Link> TopologyHelper::getLS()
{ return LS; }

/*
 * Function which generates the topology
 * (tries to arrange the MRs in a square with at most 6 neighbours)
 */
void TopologyHelper::generateTopology()
{
	//ensure we are generating a new topology
	MR.clear();
		
	//determine how far apart the MRs should be
	int mr_per_row = (int)(round(sqrt(num_mr)));
	double x_sep = max_x / mr_per_row;
	double y_sep = max_y / mr_per_row;
	
	if(x_sep > range)
		x_sep = range;
	if(y_sep > range)
		y_sep = range;
	
	//important variables for generating the topology
	int mr_count = 0;
	int current_mr_per_row = 0;
	double current_x = 0;
	double current_y = 0;
	MeshRouter temp;
	
	//std::cout << "MR_PER_ROW: " << mr_per_row << " X-SEP: " << x_sep << " Y-SEP: " << y_sep << std::endl;
	
	//continue to generate MRs while we we do not have enough
	while(mr_count < num_mr)
	{
		temp = MeshRouter(current_x, current_y, mr_count);
		MR.push_back(temp);
		current_mr_per_row++;
		mr_count++;
		
		if(current_x + x_sep >= max_x || current_mr_per_row >= mr_per_row)
		{
			current_x = 0;
			current_y += y_sep;
			current_mr_per_row = 0;
		}
		else
			current_x += x_sep;
	}
}

/*
 * Function which generates the Mesh Clients
 * (ensures that none are out of range of MRs just in case)
 */
void TopologyHelper::generateMeshClients()
{
	//ensure we are generating a new topology
	MC.clear();
	
	int mc_count = 0;
	MeshClient temp;
	//continue to generate mesh clients while we do not have enough
	while(mc_count < num_mc)
	{
		double current_x = drand48() * max_x;;
		double current_y = drand48() * max_y;;
		int closest_mr = nearestMR(current_x, current_y);
		
		//keep generating new points until we find one that is in range of the existing MRs
		while(distance(current_x, MR[closest_mr].getX(), current_y, MR[closest_mr].getY()) > range)
		{
			current_x = drand48() * max_x;
			current_y = drand48() * max_y;
			closest_mr = nearestMR(current_x, current_y);
		}
		
		temp = MeshClient(current_x, current_y, mc_count, closest_mr);
		MC.push_back(temp);
		
		mc_count++;
	}
}

/*
 * Function which generates the link paths between any given MR and
 * its closest GW
 */
void TopologyHelper::generateLinks()
{
	for(int c=0; c<(int)MR.size(); c++)
	{
		int current_gw = MR[c].getNearestGW();
		int current_router = c;
		
		//traverse each router to its gateway adding links along the way
		while(current_router != current_gw)
		{
			int nearest_neighbour = nearestNeighbour(current_router, current_gw);
			Link temp(current_router, nearest_neighbour);
			addLink(temp);
			current_router = nearest_neighbour;
		}
	}
}

/*
 * Returns the nearest neighbour between the router and the gateway
 */
int TopologyHelper::nearestNeighbour(int router, int gateway)
{
	double min_distance = std::numeric_limits<double>::max();
	int min_index = -1;
	for(int c=0; c<(int)MR.size(); c++)
	{
		if(c!=router)
		{
			double distance_to_neighbour = distance(MR[router].getX(), MR[c].getX(), MR[router].getY(), MR[c].getY());
			//std::cout << " distance from " << router << " to " << c << ": " << distance_to_neighbour << std::endl;
			if(distance_to_neighbour <= range)
			{
				if(c == gateway)
					return c;
				double dist = distance(MR[c].getX(), MR[gateway].getX(), MR[c].getY(), MR[gateway].getY());
				//std::cout << "    distance from neighbour: " << c << " to " << gateway << ":" << dist << std::endl;
				if(dist < min_distance)
				{
					min_distance = dist;
					min_index = c;
				}
			}
		}
	}
	if(min_index == -1)
	{
		std::cout << "Error, nearestNeighbour could not find a neighbour" << std::endl;
		exit(255);
	}
	return min_index;
}

/*
 * Only adds the link to the LS vector if it is unique
 */
void TopologyHelper::addLink(Link temp)
{
	for(int c=0;c<(int)LS.size();c++)
		if( (temp.first == LS[c].first && temp.second == LS[c].second) || (temp.first == LS[c].second && temp.second == LS[c].first) )
			return;
	LS.push_back(temp);
}

/*
 * Positions the gateways evenly througout the mesh network
 */
void TopologyHelper::positionGWs()
{
	//automatically assign the middle mesh router as a gateway to start
	int middle_mr = (MR.size()-1) / 2;
	MR[middle_mr].setGW();
	GW.push_back(MR[middle_mr]);
	
	//keep selecting gateways while we still need more
	while((int)GW.size() < num_gw)
	{
		int next_gw = getNextGW();
		MR[next_gw].setGW();
		GW.push_back(MR[next_gw]);
	}
	
	//associate each MR with its closest GW
	for(int c=0;c<(int)MR.size();c++)
	{
		int nearest_gw = nearestGW(c);
		int hops = distance(MR[c].getX(), MR[nearest_gw].getX(), MR[c].getY(), MR[nearest_gw].getY()) / range;
		MR[c].setHops(hops);
		MR[c].setNearestGW(nearest_gw);
	}
}

/*
 * This will select the best MR for another GW given the GWs that
 * already exist by maximizing the minimum distance from all GWs
 */
int TopologyHelper::getNextGW()
{
	double min_distance = 0;
	int min_index = -1;
	
	for(int c=0; c<(int)MR.size(); c++)
	{
		int nearest_gw = nearestGW(c);
		double current_distance = distance(MR[c].getX(), MR[nearest_gw].getX(), MR[c].getY(), MR[nearest_gw].getY());
		if(current_distance > min_distance)
		{
			min_distance = current_distance;
			min_index = c;
		}
	}
	
	if(min_index == -1)
	{
		std::cout << "Error, could not choose the next best GW, serious problems." << std::endl;
		exit(255);
	}
	
	return min_index;
}

/*
 * Returns the nearest GW to a given MR
 */
int TopologyHelper::nearestGW(int mr)
{
	double min_distance = std::numeric_limits<double>::max();
	int min_index = -1;
	for(int c=0; c<(int)GW.size();c++)
	{
		double current_distance = distance(MR[mr].getX(), GW[c].getX(), MR[mr].getY(), GW[c].getY());
		if(current_distance < min_distance)
		{
			min_index = c;
			min_distance = current_distance;
		}
	}
	
	if(min_index == -1)
	{
		std::cout << "Error, could not find a nearest GW, serious problems." << std::endl;
		exit(255);
	}
	
	return GW[min_index].getId();
}

/*
 * Returns the nearest MR to a given x,y point
 */
int TopologyHelper::nearestMR(double x, double y)
{
	double min_distance = std::numeric_limits<double>::max();
	int min_index = -1;
	for(int c=0; c<(int)MR.size();c++)
	{
		double current_distance = distance(x, MR[c].getX(), y, MR[c].getY());
		if(current_distance < min_distance)
		{
			min_index = c;
			min_distance = current_distance;
		}
	}
	
	if(min_index == -1)
	{
		std::cout << "Error, could not find a nearestMR, serious problems." << std::endl;
		exit(255);
	}
	return min_index;
}

/*
 * Returns the distance between two 2d points p1, p2
 */
double TopologyHelper::distance(double p1x, double p2x, double p1y, double p2y)
{
	return sqrt((p2x-p1x)*(p2x-p1x) + (p2y-p1y)*(p2y-p1y));
}
#endif
