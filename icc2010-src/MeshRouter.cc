/*
 * C++ Discrete Event Simulation
 * for Wireless Mesh Networks
 * by Jason Ernst
 * University of Guelph, Guelph ON, Canada
 * Version 0.3: December 2008
 * 
 * MeshRouter.cc
 * - Representation of MeshRouters and Gateways in the Network
 */

#ifndef MESHROUTER_CC
#define MESHROUTER_CC

#include "MeshRouter.h"

MeshRouter::MeshRouter()
{
	id = -1;
	x=0.0;
	y=0.0;
	gw = false;	
	nearest = -1;
	buffer = -1;
	permission = false;
}

/*
 * Constructor for Mesh Router which accepts all the necessary parameters
 */
MeshRouter::MeshRouter(double _x, double _y, int _id)
{
	id = _id;
	x = _x;
	y = _y;
	gw = false;
	nearest = -1;
	buffer = -1;
	permission = false;
}

/*
 * Displays all the important information within a MR
 */
void MeshRouter::display()
{
	std::cout << "    MR:" << id << " X: " << x << " Y: " << y << " isGW: " << gw << " NearestGW: " << nearest << " #Hops: " << hops << " PER: " << permission << " BUF: " << buffer << std::endl;
}

/*
 * Sets the buffer of the MR to the new contents
 */
void MeshRouter::setBuffer(int _buffer)
{
	buffer = _buffer;
}

/*
 * Sets the router's tranmission permission to true or false
 */
void MeshRouter::setPermission(bool _permission)
{
	permission = _permission;
}

/*
 * Sets a given Mesh Router as a Gateway
 */
void MeshRouter::setGW()
{
	gw = true;
}

/*
 * Sets the nearest GW to the MR where all of the packets will be routed to
 */
void MeshRouter::setNearestGW(int _nearest)
{
	nearest = _nearest;
}

/*
 * Sets the number of hops to the nearest GW
 */
void MeshRouter::setHops(int _hops)
{
	hops = _hops;
}

/*
 * Returns the contents of the MR buffer
 */
int MeshRouter::getBuffer()
{
	return buffer;
}

/*
 * Returns the ID of the Mesh Router
 */
int MeshRouter::getId()
{
	return id;
}

/*
 * Returns the x-coordinate of the Mesh Router
 */
double MeshRouter::getX()
{
	return x;
}

/*
 * Returns the y-coordinate of the Mesh Router
 */
double MeshRouter::getY()
{
	return y;
}

/*
 * Returns true if the Mesh Router is a Gateway
 */
bool MeshRouter::isGW()
{
	return gw;
}

/*
 * Returns true if the router has permission to transmit
 */
bool MeshRouter::hasPermission()
{
	return permission;
}

/*
 * Returns the nearest GW to the given Mesh Router
 */
int MeshRouter::getNearestGW()
{
	return nearest;
}

/*
 * Returns the number of hops to the nearest GW
 */
int MeshRouter::getHops()
{
	return hops;
}
#endif
