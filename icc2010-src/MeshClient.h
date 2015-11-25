#ifndef MESHCLIENT_H
#define MESHCLIENT_H

#include <iostream>

class MeshClient
{
	public:
		MeshClient();
		MeshClient(double _x, double _y, int _id, int _mr);
		
		void display();
		
		int getId();
		int getMR();
		double getX();
		double getY();
		
		~MeshClient();
	private:
		int id, mr;
		double x;
		double y;
};

#endif
