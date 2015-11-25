/*
 * C++ Discrete Event Simulation
 * for Wireless Mesh Networks
 * by Jason Ernst
 * University of Guelph, Guelph ON, Canada
 * Version 0.3: December 2008
 * 
 * Mesh Client Source Code
 */

#ifndef MESHCLIENT_CC
#define MESHCLIENT_CC
#include "MeshClient.h"

MeshClient::MeshClient()
{
	id = -1;
	x = -1;
	y = -1;
	mr = -1;
}

/*
 * Constructor for a Mesh Client which takes all of the required
 * parameters a Mesh Client needs.
 */
MeshClient::MeshClient(double _x, double _y, int _id, int _mr)
{
	x = _x;
	y = _y;
	id = _id;
	mr = _mr;
}

void MeshClient::display()
{
	std::cout << "    MC:" << id << " X: " << x << " Y: " << y << " Nearest MR: " << mr << std::endl;
}

int MeshClient::getMR()
{
	return mr;
}

int MeshClient::getId()
{
	return id;
}

double MeshClient::getX()
{
	return x;
}

double MeshClient::getY()
{
	return y;
}

MeshClient::~MeshClient()
{}

#endif
