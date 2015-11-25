#ifndef TOPOLOGYHELPER_H
#define TOPOLOGYHELPER_H

#include <iostream>
#include <cmath>
#include <limits>

#include "MeshRouter.h"
#include "MeshClient.h"
#include "Link.h"

class TopologyHelper
{
	public:
		TopologyHelper();
		TopologyHelper(int _max_x, int _max_y, int _num_mr, int _num_gw, int _num_mc, double _range);
		
		std::vector<MeshRouter> getMR();
		std::vector<MeshRouter> getGW();
		std::vector<MeshClient> getMC();
		std::vector<Link> getLS();
	
	private:
		void generateTopology();
		void generateMeshClients();
		void generateLinks();
		void positionGWs();
		void addLink(Link temp);
		
		int getNextGW();
		int nearestGW(int mr);
		int nearestMR(double x, double y);
		int nearestNeighbour(int router, int gateway);
		double distance(double p1x, double p2x, double p1y, double p2y);
		
		int max_x, max_y, num_mr, num_gw, num_mc;
		double range;
		std::vector<MeshRouter> MR;
		std::vector<MeshRouter> GW;
		std::vector<MeshClient> MC;
		std::vector<Link> LS;
};

#endif
