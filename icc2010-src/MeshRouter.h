#ifndef MESHROUTER_H
#define MESHROUTER_H

#include <vector>
#include <iostream>
#include <stdlib.h>

class MeshRouter
{
	public:
		MeshRouter();
		MeshRouter(double _x, double _y, int _id);
		
		void setPermission(bool _permission);
		void display();
		void setBuffer(int _buffer);
		void setGW();
		void setNearestGW(int _nearest);
		void setHops(int _hops);
		
		bool hasPermission();
		bool isGW();
		double getX();
		double getY();
		int getId();
		int getNearestGW();
		int getHops();
		int getBuffer();
		
	private:
		double x,y;
		int id, nearest;
		int hops;
		int buffer;
		bool gw, permission;
};

#endif
